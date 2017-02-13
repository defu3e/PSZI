#include "headall.h"

#define REGPATH "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\"


U8 on_autorun ()
{
DWORD   value=0xff;
HKEY    hk;

RegCreateKeyEx(HKEY_LOCAL_MACHINE, REGPATH, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hk, NULL);

RegSetValueEx ( hk,                          // ����������
                "NoDriveTypeAutoRun",        // �������� �����
                0,                           // 0
                REG_DWORD,                   // ��� �����
                (const BYTE*)&value ,        // ��������� �� ������
                sizeof(value)                // ����� ������
               );

RegCloseKey(hk);

if (MessageBox(NULL, "\n��� ���������� ��������� � ���� ���������� ������������� ���������\n������������� ������?\n",
                   "���������� ������� ��������� ��������.", MB_YESNO|MB_ICONQUESTION) == IDYES)
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

        if (MessageBox(NULL, "\n��� ���������� ��������� � ���� ���������� ������������� ���������\n������������� ������?\n",
                           "���������� ������� ��������� ��������.", MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            return 1;
        }

        return 0;
}
