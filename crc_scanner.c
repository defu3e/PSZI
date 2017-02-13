#include "headall.h"
#include "crc.h"
#include <string.h>

#define CRCBASE     "crcbase.pszib"
#define CRCSTAT     "CRCSTAT.txt"

U8              object_size         (const char *path);
U8              object_extension    (const char * fname);
unsigned int    Crc32_file          (char *crc32file_address);


/*==============Статистика сканирования==============*/
struct crc_stat_list
{
    unsigned long long   all_ob;     //max 18446744073709551615
    unsigned short       infect_ob;  //max 65535
    unsigned short       delete_ob;  //max 65535
} csl;
/*===================================================*/

U8 crc_scan1 (const char * sDir)
{
    U8 result;

    FILE *crcbase = fopen(CRCBASE, "a");

    if (crcbase == NULL)
    {
        errors_list(3, CRCBASE);
        return 1;
    }

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[MAX_PATH];

    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf("\n....................................\n[%s]\n", sDir);
        return 0;
    }

    do
    {
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {

            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
                crc_scan1(sPath);
            else
            {
                if (object_size(sPath))
                {
                    if (!object_extension(fdFile.cFileName) )
                    {
                        unsigned long crc_sum = Crc32_file(sPath);

                        if (crc_sum == -1)
                        {
                            printf ("Невозможно посчитать CRC");
                            return 1;
                        }

                        fprintf(crcbase, "%X\n", crc_sum);
                        printf("Объект: %s\n", sPath);
                        printf ("CRC: %X\n", crc_sum );
                    }
                }
            }
        }
    }
    while(FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    fclose(crcbase);

    return 1;

}

U8 crc_scan (const char * sDir)
{
    U8 i, result;

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[MAX_PATH];

    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf(".....................................\n[%s]\n", sDir);
        return 0;
    }

    do
    {
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
        {

            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
                crc_scan(sPath);
            else
            {
                if (!object_size(sPath))
                {
                    if (object_extension(fdFile.cFileName) )
                    {
                        unsigned long crc_sum = Crc32_file(sPath);
                        FILE *crcbase = fopen(CRCBASE, "r");
                        unsigned long crc_sum_base;
                        BOOL CRCfound = FALSE;
                        csl.all_ob++;

                        if (crc_sum == -1)
                        {
                            printf ("Невозможно посчитать CRC");
                            return 1;
                        }

                    while (fscanf(crcbase, "%X", &crc_sum_base) != EOF)
                    {
                        if (crc_sum == crc_sum_base)
                        {
                            CRCfound = TRUE;
                        }
                    }

                    if (!CRCfound)
                    {
                        printf("\n%X == %X", crc_sum, crc_sum_base);
                        csl.infect_ob++;
                        neutralize_object(sPath, crc_sum);
                        return 1;
                    }

                        printf("Объект: %s\n", sPath);
                        printf ("CRC: %X\n", crc_sum );
                        fclose(crcbase);
                    } // while
                }
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return 1;

}

neutralize_object (const char *path, unsigned long crc_sum)
{
    char mes[] = "Объект может быть заражён\nУдалить файл?\n\nПримечание: Если вы доверяете файлу, нажмите 'Нет'\n\n";
    char dialog_text[strlen(path)+strlen(mes)+1];
    strcpy(dialog_text,mes);
    strcat(dialog_text,path);

    if (MessageBox(NULL, dialog_text,"Возможная угроза", MB_YESNO|MB_ICONERROR) == IDYES)
    {
        if(DeleteFile(path)==0)
            errors_list(2, path);
        else
        {
            printf("Объект %s успешно удалён\n", path);
            csl.delete_ob++;
        }
    }
    else
    {
        FILE *crcbase = fopen (CRCBASE,"a");
        if (crcbase == NULL)
        {
            errors_list(3, CRCBASE);
            return 1;
        }

        fprintf(crcbase, "%X\n", crc_sum);

        fclose(crcbase);
    }
    return 0;
}


U8 object_size (const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
    DWORD   oSize;
    uint8_t result;

    GetFileAttributesEx(path, 0, &fileInfo);

    oSize = fileInfo.nFileSizeLow;

    result = (oSize > 1000000000 || oSize < 1000000 ) ? 1 : 0;  // > 1Мб && < 4 Гб

    return result;
}

U8 object_extension (const char * fname)
{
    char extTable[8][9] = {"EXE\0", "COM\0", "BAT\0", "JS\0",
                           "CMD\0", "JAR\0", "VBS\0", "VBE\0"};
    char *pend = fname;
    char ext[9];
    U8 i, j;

    U8 len = strlen(fname)-1;
    pend += len;


    for (i=len, j=0; i!= 0; i--)
    {
        if (*pend != '.')
        {
            ext[j++] = *pend;
            ext [j]  = '\0';
            pend--;
        } else break;
    }

    char temp [strlen(ext)];
    for (i = strlen(ext)-1, j=0; j<strlen(ext); i--,j++)
        temp[i] = ext[j];

    for (i = 0; i < strlen(ext); i++)
        ext[i] = toupper(temp[i]);

    for (i = 0; i<strlen(extTable); i++)
        if (strcmp (extTable[i], ext) ==0) return 1;

    return 0;
}

U8 start_crc_scanner ()
{

BOOL first_scan = FALSE;

if (access(CRCBASE, 0))
{
    if (           MessageBox(NULL,
                   "CRC-сканирование никогда не запускалось\nЗапустить первичную проверку компьютера?",
                   "Нет базы CRC-сумм",
                    MB_YESNO|MB_ICONINFORMATION
                    ) == IDYES
        )
        first_scan = TRUE;
    else return 1;
}

    csl.all_ob = 0;
    csl.infect_ob = 0;
    csl.delete_ob = 0;


    AllocConsole();                     //http://vsokovikov.narod.ru/New_MSDN_API/Console/con_fn.htm
    SetConsoleCP(1251);                 //system("chcp 1251");
    SetConsoleOutputCP(1251);
    freopen( "CON", "w", stdout) ;
    //AllocConsole();
    //freopen("CONOUT$", "wb", stdout);



U8 n, i, j = 0;;
char letter[1];
char path [3];
DWORD dr = GetLogicalDrives();  //получаем битовую маску доступных дисков, в том числе и съемных
for(i = 0; i < 26; i++ )        //потому что всего 26 букв
{
    n = ((dr>>i)&0x00000001);   //сдвигаемся по битам
    if( n == 1 )
    {
        letter[0] = (char) i+65;          //буквы в таблице начинаются с 65 символа( A )
        printf ("%c\n", letter[0]);
        path [j++] = letter[0];
        path [j++]  = ':';
        path [j++]  = '\0';
        j=0;
        if (first_scan)
            crc_scan1(path);
        else
            crc_scan(path);
    }
}
    FreeConsole();
     /*========== Запись статистики сканирования в файл MD5-Scan stat.txt=========*/
            /*==========================================================================*/
            FILE *stats = fopen (CRCSTAT, "a+");
            if(stats == NULL)
            {
                errors_list(8, CRCSTAT);
                return 0;
            }

            fprintf(stats, "------ %s ------\nПросканировано файлов: %lu\nОбнаржуено вирусов: %d\nУдалено: %d\n\n",
                    date_str(),
                    csl.all_ob,
                    csl.infect_ob,
                    csl.delete_ob
                    );

            fclose(stats);
    /*=========================================================================*/

return 0;
}
