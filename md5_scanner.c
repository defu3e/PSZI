/**
*
*http://virusshare.com/hashes.4n6
*/
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include "headall.h"
#include "md5.h"

#define MD5BASE  "MD5BASE.pszib"
#define MD5STAT  "MD5STAT.txt"
#define HASH_LEN 32

/**************************************************************************************/
/******************************Прототипы функий****************************************/
U8      hash_scan    (const char * sDir, U8 mode);
U8      detect_virus (const char * path);
U8      delete_virus (const char * path);
void    errors_list  (U8 ecode, char *param);
void    close_console(void);
/********Внешние функции*********/
extern  U8      object_size  (const char * path);
/*****************************/

/**************************************************************************************/

/*==============Статистика сканирования==============*/
struct md5_stat_list
{
    unsigned long long   all_ob;     //max 18446744073709551615
    unsigned short       infect_ob;  //max 65535
    unsigned short       delete_ob;  //max 65535
} msl;
/*===================================================*/


U8 hash_scan(const char *sDir, U8 mode)
{
    WIN32_FIND_DATA object;
    char sPath[MAX_PATH];

    HANDLE hFind = NULL;

    sprintf(sPath, "%s\\*.*", sDir);

/*============================================ Проверка файла ========================================================*/
/*====================================================================================================================*/
    if((hFind = FindFirstFile(sPath, &object)) == INVALID_HANDLE_VALUE)
    {

        if (object_size(sDir))          // Размер файла больше 1 ГБ или меьшне 1 Мб
        {
            errors_list(0, sDir);
            return 1;
        }

        U8 md5res = detect_virus (sDir);

        if (md5res == 2)
            delete_virus(sDir);
        else
        {
                FreeConsole();
                MessageBox(NULL, "Файл безопасен", "Сканирование завершено", MB_OK | MB_ICONASTERISK);
        }
        return 0;
    }
/*============================================ Проверка каталога =====================================================*/
/*====================================================================================================================*/
    do
    {
        if(strcmp(object.cFileName, ".") != 0 && strcmp(object.cFileName, "..") != 0) // Пропуск первых двух директорий: *..* и *.*
        {
            sprintf(sPath, "%s\\%s", sDir, object.cFileName);

            if(object.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)   // Определить тип объекта
            {
                printf("Каталог: %s\n", sPath);
                hash_scan(sPath, 0);                                // рекурсивный запуск
            }
            else
            {
                if (object_size(sPath))                 // Пропуск файлов размера 1 Гб и < 1 Мб
                    continue;

                if (!object_extension(object.cFileName))
                    continue;

                printf("Объект: %s \n", sPath);
                msl.all_ob++;

                if (detect_virus (sPath)==2)            // Проверка на совпадение хешей
                    delete_virus(sPath);
            }
        }
    } // do
    while(FindNextFile(hFind, &object)); // пока находится файл

    FindClose(hFind); // очистка временных файлов поиска

    return 0;
}


void SetStdOutToNewConsole()
{
    int     hConHandle;
    long    lStdHandle;
    FILE    *fp;

    AllocConsole();
    // перенаправить STDOUT в консоль
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, O_TEXT);
    fp         = _fdopen( hConHandle, "w" );
    *stdout    = *fp;

    setvbuf( stdout, NULL, _IONBF, 0 );
}

/*======================================== Поиск файла в базе сигнатур ===============================================*/
/*====================================================================================================================*/
U8 detect_virus (const char * path)
{
    char *MD5_hash = MD5_file(path);    // запись MD5 файла
    char virus_hash[HASH_LEN+1];        // хэш вируса из сигнатуры
    FILE *md5base;

    md5base = fopen(MD5BASE, "r");      // открытие файла

    if(md5base == NULL)
    {
        errors_list(1, MD5BASE);
        return 1;
    }

    while (fgets(virus_hash, HASH_LEN+1, md5base) != NULL)
    {
        if (!strcmp(virus_hash, MD5_hash))        // если есть совпадение
        {
            fclose (md5base);                     // закрываем поток работы с файлом
            printf("! MD5-хэш %s найден в сигнатурах вирусов\n", MD5_hash);
            return 2;                             // найден вирус
        }
    }

    fclose (md5base);                             // закрыть поток работы с файлов

    return 0;
}
/*====================================================================================================================*/

/*======================================== Удаление вредоносного файла ===============================================*/
/*====================================================================================================================*/
U8 delete_virus (const char * path)
{
    msl.infect_ob++;
    char mes[] = "Удалить инфицированный объект?\n";
    char dialog_text[strlen(path)+strlen(mes)];
    strcpy(dialog_text,mes);
    strcat(dialog_text,path);

    if (MessageBox(NULL, dialog_text,"Обнаружен вирус!", MB_YESNO|MB_ICONERROR) == IDYES)
    {
        if(-1 == DeleteFile(path))
            errors_list(2, path);
        else
        {
            printf("Объект %s успешно удалён\n", path);
            msl.delete_ob++;
        }
    }
    return 0;
}
/*====================================================================================================================*/

/*======================================== Получение текщей даты и времени ===========================================*/
/*====================================================================================================================*/
char *date_str()
{
    char    date_str[MAX_PATH];
    struct  tm *tmp;
    time_t  t = time(NULL);

    tmp = localtime(&t);
    if (tmp == NULL)
    {
       perror("localtime");
       exit(EXIT_FAILURE);
    }

    if (strftime(date_str, sizeof(date_str), "%x %X" , tmp) == 0)
    {
       fprintf(stderr, "strftime ret 0");
       exit(EXIT_FAILURE);
    }
   return date_str;
}
/*====================================================================================================================*/

/*========================================== Начало сканирования =====================================================*/
/*====================================================================================================================*/
U8 start_md5_scanner (char *path, U8 md5_scan_mode)
{
    setlocale (LC_ALL, "rus");
/*======== Проверка корректности пути ========*/
    if (GetFileAttributes(path) == -1)  // если путь неверный
    {
        errors_list(4, path);
        return 1;
    }
/*============================================*/
/*====== Проверка файла сигнатур вирусов =====*/
if (access(MD5BASE, 0))
{
    errors_list (1, MD5BASE);
    return 1;
}


/*============================================*/

/*========== Подключение терминала вывода ==========*/
SetStdOutToNewConsole();
/*=================================================*/
    if (md5_scan_mode)
    {
        printf      ("[%s]\n", path);
        hash_scan   (path, md5_scan_mode);
        goto        M_EXIT;
    }

    msl.all_ob      = 0;
    msl.infect_ob   = 0;
    msl.delete_ob   = 0;

   hash_scan (path, md5_scan_mode);

/*========== Запись статистики сканирования в файл MD5-Scan stat.txt=========*/
/*==========================================================================*/

    FILE *stats = fopen (MD5STAT, "a+");
    if(stats == NULL)
	{
		errors_list(8, MD5STAT);
		return 0;
	}

    fprintf (stats, "\n------ %s ------\nПуть: %s\n", date_str(), path);
    fprintf (stats, "Просканировано файлов: %lu\n", msl.all_ob);
    fprintf (stats, "Обнаружено вирусов: %d\n", msl.infect_ob);
    fprintf (stats, "Удалено: %d\n", msl.delete_ob);

    fclose(stats);

    FreeConsole();
    MessageBox(NULL, "Сигнатурное сканирование завершено", "MD5-сканер", MB_OK|MB_ICONINFORMATION);

/*================================================EXIT===============================================================*/
M_EXIT:
    FreeConsole();
    return 1;
}
/*====================================================================================================================*/


