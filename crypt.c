#include "headall.h"
#include "crypt.h"
#include <string.h>

#define KEY "TESTKEY"

/*=========================================================================================*/
void strrem( char* _pSourceStr, const char* _pDelStr );
U8 checkExt(const char *path, const char *e_ext);
/*=========================================================================================*/
/*=========================================================================================*/

WIN32_FIND_DATA wfd;	//Структура WIN32_FIND_DATA описывает файл,

U8 corFile(path)
{
    FILE *file = fopen (path, "ab+");
    if (file == NULL)
    {
        errors_list(7, path);
        return 1;
    }

    unsigned int a = 0;

    fwrite((void*)&a, sizeof(int), 1, file);
    fwrite((void*)&a, sizeof(int), 1, file);

    fclose(file);
    return 0;
}

U8 operateFile(char *file, char *file2, char mode)
{
	HANDLE hFile;

	hFile = FindFirstFile(file, &wfd);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Ошибка файла!!!\n");
		return 0;
	}

	int size = wfd.nFileSizeLow;
	int total_read = 0;

	unsigned long buffer;
	unsigned int flag = 1;

	FILE *f = fopen(file, "rb");
	FILE *f2 = fopen(file2, "wb");

	if (f == NULL || f2 == NULL)
	{
		printf("ERROR open file!!!\n");
		return 0;
	}

	switch (mode)
	{
		case 'e':
			while (fread(&buffer, 4, 1, f))
			{
				if (flag == 1)
				{
					high = buffer;
					flag = 2;
				}
				else
				{
					low = buffer;
					flag = 1;
					blowfish_encrypt_block(ctx, &high, &low);
					fwrite(&high, 4, 1, f2);
					fwrite(&low, 4, 1, f2);
				}

				total_read += 4;
			}

			break;

		case 'd':
			while (fread(&buffer, 4, 1, f))
			{
				if (flag == 1)
				{
					high = buffer;
					flag = 2;
				}
				else
				{
					low = buffer;
					flag = 1;
					blowfish_decrypt_block(ctx, &high, &low);
					fwrite(&high, 4, 1, f2);
					fwrite(&low, 4, 1, f2);
				}
				total_read += 4;
			}
			break;

		default:
			printf("ERROR Work-Mode!!!\n");
			return 0;
	}

	fclose(f);
	fclose(f2);

	return 1;
}

U8 start_crypt (const char *path, U8 mode)
{
/*=============== Проверка существования файла =============*/
if (access(path, 0))
{
    errors_list (5, path);
    return 1;
}
/*==========================================================*/

    unsigned char * key = KEY;

    if (GetFileAttributes(path) == -1)  // если путь неверный
    {
        errors_list(5, path);
        return 1;
    }

    ctx = (blowfish_ctx *)malloc(sizeof(blowfish_ctx));

	if (!ctx)
	{
		printf("Выделено недостаточно памяти");

		return 1;
	}

	blowfish_init(ctx, key, strlen(key));


    char e_ext[] = ".pszie";

    if (mode)
    {
        char *newPath[strlen(path)+strlen(e_ext)];
        strcpy (newPath, path);
        //strrem(newPath, )
        strcat (newPath, e_ext);
        corFile(path);
        operateFile(path, newPath, 'e');

        if (MessageBox(NULL, "Удалить исходный файл?", "Удаление исходного файла", MB_YESNO|MB_ICONQUESTION) == IDYES)
            if(DeleteFile(path) == 0) errors_list(3, path);

        MessageBox(NULL, "Операция шифрования успешно завершена\n", "Шифрование", MB_OK|MB_ICONINFORMATION);
    }
    else
    {
        if (checkExt(path, e_ext))
        {
            errors_list (6,path);
            return 1;
        }
        char oldPath[MAX_PATH];
        strcpy (oldPath, path);
        strrem (oldPath, e_ext);

        operateFile(path, oldPath, 'd');
        MessageBox(NULL, "Операция расшифрования успешно завершена\n", "Расшифровка", MB_OK|MB_ICONINFORMATION);
    }


	free(ctx);

	return 0;
}


void strrem( char* _pSourceStr, const char* _pDelStr )
{
	char* pTmpStr;

	pTmpStr = strstr( _pSourceStr, _pDelStr );

	strcpy( pTmpStr, pTmpStr + strlen( _pDelStr ) );
};

U8 checkExt(const char *path, const char *e_ext)
{

    char *extStart = strchr (path, '.');
    if (extStart == NULL)
        return 1;
    if (strcmp (extStart, e_ext) == 0)
        return 0;
    else
        return 1;
}


