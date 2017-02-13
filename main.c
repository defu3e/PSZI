#include "headall.h"

#define REGPATH "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\"


U8 on_autorun ()
{
DWORD   value=0xff;
HKEY    hk;

RegCreateKeyEx(HKEY_LOCAL_MACHINE, REGPATH, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hk, NULL);

RegSetValueEx ( hk,                          // дескриптор
                "NoDriveTypeAutoRun",        // название ключа
                0,                           // 0
                REG_DWORD,                   // тип ключа
                (const BYTE*)&value ,        // указатель на данные
                sizeof(value)                // длина данных
               );

RegCloseKey(hk);

if (MessageBox(NULL, "\nƒл¤ вступлени¤ изменений в силу необходимо перезагрузить компьютер\nѕерезагрузить сейчас?\n",
                   "јвтозапуск съЄмных носителей отключен.", MB_YESNO|MB_ICONQUESTION) == IDYES)
    return 1;
    return 0;
}


U8 off_autorun ()
{
    HKEY hk;

    RegOpenKey(HKEY_LOCAL_MACHINE, REGPATH, &hk);

    if (RegDeleteKey(hk, "") != ERROR_SUCCESS)
    {
        errors_list(11, REGPATH);
        return 2;
    }
        RegCloseKey(hk);

        if (MessageBox(NULL, "\nƒл¤ вступлени¤ изменений в силу необходимо перезагрузить компьютер\nѕерезагрузить сейчас?\n",
                           "јвтозапуск съЄмных носителей отключен.", MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            return 1;
        }

        return 0;
}
