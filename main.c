#include "headall.h"
//#include <commctrl.h>       // для управляющих элментов
#include <Shlobj.h>
#include <Commctrl.h>
#include <Uxtheme.h>
#include <Windowsx.h>
#include <unistd.h>

#define WND_CLASS_NAME0 "my_wnd_class_name_0"
#define PWORD "1783SevGU"

#define IDC_GROUP5     2000
#define IDC_GROUP6     2001
#define IDC_BUTTON6   2002
#define IDC_BUTTON5   2003
#define IDC_STATIC3   2004
#define IDC_GROUP4    2005
#define IDC_BUTTON4   2006
#define IDC_BUTTON3   2007
#define IDC_RADIO3    2008
#define IDC_RADIO2    2009
#define IDC_STATIC2   2010
#define IDC_GROUP3    2011
#define IDC_STATIC1   2012
#define IDC_GROUP2    2013
#define IDC_BUTTON2   2014
#define IDC_BUTTON1   2015
#define IDC_EDIT1     2016
#define IDC_EDIT0     2017
#define IDC_EDIT2     2018
#define IDC_BUTTON0   2019
#define IDC_STATIC0   2020
#define IDC_RADIO1    2021
#define IDC_RADIO0    2022
#define IDC_CHECK0    2023
#define IDC_GROUP1    2024
#define TAB0          2025
#define IDC_GROUP0    2026
#define IDC_STATIC4   2027
#define IDC_STATIC5   2028
#define IDC_STATIC6   2029
#define IDC_STATIC7   2030
#define IDC_STATIC8   2031
#define IDC_STATIC9   2032
#define IDC_STATIC10   2032
#define IDC_STATIC11   2033
#define IDC_STATIC12   2034
#define IDC_STATIC13   2035
#define IDC_STATIC14   2036
#define IDC_STATIC15   2037
#define IDC_STATIC16   2038
#define IDC_STATIC17   2039
#define IDC_STATIC18   2040
#define IDC_STATIC19   2041
#define IDC_STATIC20   2042
/**************************Прототипы функций*********************************/
/***************************************************************************/
LRESULT CALLBACK wnd_proc0  (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void    create_wnd_content0 (HWND parent);
void    register_classes    ();
int     message_loop        ();
HWND    create_wnd_0        ();
/***************************************************************************/

/*************************Глобальные переменные*****************************/
/***************************************************************************/
HINSTANCE   instance;
HFONT       h_font;
/***************************************************************************/

int WINAPI WinMain(HINSTANCE hinstance,     //  дескриптор экземпляра
                   HINSTANCE hprevinst,     //  не испрользуется
                   LPSTR cmdline,           //  указатель на параметы командной строки
                   int show)                //  код состояния окна
{
instance = hinstance;
register_classes();
create_wnd_0();

return message_loop();
}

/*==============================================================================================================================*/
/*=========================================== Обработка сообщений главного окна ================================================*/
LRESULT CALLBACK wnd_proc0(HWND hwnd,       // дескриптор окна
                           UINT msg,        // код сообщения
                           WPARAM wparam,   // 32-битные пара
                           LPARAM lparam)   // метры
{

/*=======================================================================*/
char            szFile[MAX_PATH], // имя файла
                strNum[MAX_PATH], // путь сканирования
                pwStr[strlen(PWORD)];
TCHAR           szDir[MAX_PATH];
U8              offRunResult;
HWND            hf,
                hEdit_1,
                hEdit_2,
                hEdit_3,
                hRadio;
BROWSEINFO      bInfo;          // открытие каталога
OPENFILENAME    ofn;            // открытие файла
LPITEMIDLIST    lpItem;         // для просмотра содержимого окна
HBRUSH          br;             // кисть
/*=======================================================================*/
hEdit_1 = GetDlgItem(hwnd, IDC_EDIT0); // дескрипторы поля выбора файла
hEdit_2 = GetDlgItem(hwnd, IDC_EDIT1);
hEdit_3 = GetDlgItem(hwnd, IDC_EDIT2);
/*=======================================================================*/
switch (msg)
{
    case WM_COMMAND:
        switch (LOWORD(wparam))
        {
            case IDC_BUTTON6:
                offRunResult = off_autorun();
                if (offRunResult==2)
                    break;
                else
                    if (offRunResult==1)
                        {
                            system("shutdown -r -t 15");
                            FatalAppExitA(0, "Компьютер будет скоро перезагружен...");
                        }
            break;

            case IDC_BUTTON5:
                if (on_autorun())
                {
                    system("shutdown -r -t 15");
                    FatalAppExitA(0, "Компьютер будет скоро перезагружен...");
                }
            break;

            case IDC_BUTTON1:
                GetWindowText(hEdit_1,strNum,MAX_PATH);         // считать путь
                if (IsDlgButtonChecked(hwnd, IDC_RADIO1))       // если скан_файл
                    start_md5_scanner (strNum, 1);
                else
                    start_md5_scanner (strNum, 0);
			break;

            case IDC_BUTTON4:
                ZeroMemory(&ofn, sizeof(ofn));          // выделение памяти
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = szFile;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szFile);
                if (IsDlgButtonChecked(hwnd, IDC_RADIO3))   ofn.lpstrFilter = "PSZI files(*.pszie)\0*.pszie\0\0";
                else                                        ofn.lpstrFilter = "All\0*.*\0\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

                if (GetOpenFileName(&ofn)==TRUE)
                    hf = CreateFile(ofn.lpstrFile,
                                    NULL,               // Тип досупа к объекту
                                    0,
                                    (LPSECURITY_ATTRIBUTES) NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    (HANDLE) NULL
                                    );

                SendMessage(hEdit_2, WM_SETTEXT, MAX_PATH, (LPARAM)ofn.lpstrFile);
            break;

            case IDC_BUTTON3:
                GetWindowText(hEdit_2, strNum, MAX_PATH);     // считать путь
                GetWindowText(hEdit_3, pwStr, MAX_PATH);

                if (strcmp (pwStr,PWORD) != 0)      // Если пароль неверный
                {
                    if (strcmp (pwStr, "") == 0)    // если ничего не введено
                        errors_list(9, pwStr);
                    else
                        errors_list (10, pwStr);
                    return 1;
                }
                if (IsDlgButtonChecked(hwnd, IDC_RADIO3))
                    start_crypt(strNum, 0);
                else
                    start_crypt(strNum, 1);
            break;


            case IDC_BUTTON2:
                if (start_crc_scanner ()==0)
                MessageBox(NULL, "Сканирование завершено", "CRC-сканер", MB_OK|MB_ICONINFORMATION);
            break;

        case IDC_BUTTON0:
            if (IsDlgButtonChecked(hwnd, IDC_RADIO1))
            {
                ZeroMemory(&ofn, sizeof(ofn));          // выделение памяти
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwnd;
                ofn.lpstrFile = szFile;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = "All\0*.*\0\0";//"Bitmap files(*.bmp)\0*.bmp\0JPEG files(*.jpg)\0*.jpg\0All files(*.*)\0*.*\0\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

                if (GetOpenFileName(&ofn)==TRUE)
                    hf = CreateFile(ofn.lpstrFile,
                                    NULL,               // Тип досупа к объекту
                                    0,
                                    (LPSECURITY_ATTRIBUTES) NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    (HANDLE) NULL
                                    );

                SendMessage(hEdit_1, WM_SETTEXT, MAX_PATH, (LPARAM)ofn.lpstrFile);
            } // if
            else
            {
                bInfo.hwndOwner = hwnd;
                bInfo.pidlRoot = NULL;
                bInfo.pszDisplayName = szDir;
                bInfo.lpszTitle = "Выберите, пожалуйста, каталог для сканирования";
                bInfo.ulFlags = BIF_RETURNFSANCESTORS | BIF_DONTGOBELOWDOMAIN;
                bInfo.lpfn = NULL;
                bInfo.lParam = 0;
                bInfo.iImage = -1;
                lpItem = SHBrowseForFolder( &bInfo);
                if( lpItem != NULL )
                {
                    SHGetPathFromIDList(lpItem, szDir );
                    SendMessage(hEdit_1, WM_SETTEXT, MAX_PATH, (LPARAM)szDir);
                    //SendMessage(, PBM_DELTAPOS, 50, 0);//SendMessage(, PBM_STEPIT, 0, 0);
                }
            } // else
            break;
		} // swith
		break;


	case WM_NOTIFY:
		switch (((LPNMHDR) lparam)->code)
		{
            case TCN_SELCHANGING:
            case TCN_SELCHANGE:
			{
				TC_ITEM tci = { TCIF_PARAM };
				LPNMHDR pnmh = (LPNMHDR) lparam;
				int show = (pnmh->code == TCN_SELCHANGE)?SW_SHOW:SW_HIDE;
				HWND ctl = GetWindow(hwnd, GW_CHILD);
				TabCtrl_GetItem(pnmh->hwndFrom, TabCtrl_GetCurSel(pnmh->hwndFrom), &tci);
				for (; ctl; ctl = GetWindow(ctl, GW_HWNDNEXT))
					if (GetWindowLong(ctl, GWL_USERDATA) == tci.lParam) ShowWindow(ctl, show);
			}
			break;
		} // switch
		break;

	case WM_CREATE:
		create_wnd_content0(hwnd);
    break;

    case WM_CTLCOLOREDIT:
        return (LRESULT)GetSysColorBrush(COLOR_GRADIENTACTIVECAPTION);//COLOR_MENU
    break;

    case WM_CTLCOLORSTATIC:
    {
        SetTextColor(hwnd, 0x00000000); // Не работает
        SetBkMode(hwnd, TRANSPARENT); // Не работает
        SetBkColor (hwnd, 0x00FFFFFF);
        return (INT_PTR)CreateSolidBrush(0x00FFFFFF);
    }
    break;

	case WM_CLOSE:
		//if (IDYES == MessageBox(hwnd, "Quit?", "Event", MB_YESNO|MB_ICONQUESTION))
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);	// PostQuitMessage(return_code) quits the message loop.
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);

}   // main switch
	return FALSE;
} // CALLBACK wnd_proc0()


/*===========================================Создание главного окна===========================================================*/
/*============================================================================================================================*/
HWND create_wnd_0()
{
	HWND hwnd;
//	RECT screen_rect;
//    GetWindowRect(GetDesktopWindow(),&screen_rect); // разрешение экрана
//    int scr_width = screen_rect.right-screen_rect.left;
//    int scr_height = screen_rect.bottom-screen_rect.top;
    int x = (GetSystemMetrics(SM_CXSCREEN)) / 2 - 300;
    int y = (GetSystemMetrics(SM_CYSCREEN)) / 2 - 300;

	hwnd = CreateWindowEx( WS_EX_APPWINDOW,                 // Активизирует окно на панели задач
                          WND_CLASS_NAME0,                  // указатель на зарегистрированное имя класса
                          "ПСЗИ 0.1",                       // указатель на имя окна
                          WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
                          x,                                // положение по X
                          y,                                // положение по Y
                          583,                              // высота
                          430,                              // и ширина создаваемого окна
                          NULL,                             // Идентифицирует окно родителя
                          NULL,                             // меню
                          instance,                         // экземпляр модуля, который связан с окном
                          NULL);                            // значение, переданное окну через структуру CREATESTRUCT

	ShowWindow(hwnd, SW_SHOW);        // Отобразить окно
	UpdateWindow(hwnd);               // Перерисовать окно
	return hwnd;
}
/*============================================================================================================================*/

/*============================================ Создание контента окна ===========================================================*/
/*===============================================================================================================================*/
void create_wnd_content0(HWND parent)
{
	HWND wnd;
    HFONT h_font = CreateFont(
                              18,                       // высота шрифта
                              0,                        // средняя ширина символа
                              0,                        // угол наклона
                              0,                        // угол ориентации базисной линии
                              500,                      // толщина шрифта
                              FALSE,                    // описатель параметра курсивного шрифта
                              FALSE,                    // описатель параметра подчеркивания
                              FALSE,                    // описатель параметра зачеркивания
                              RUSSIAN_CHARSET,          // идентификатор набора символов
                              OUT_OUTLINE_PRECIS,       // точность вывода
                              CLIP_DEFAULT_PRECIS,      // точность отсечения
                              DEFAULT_QUALITY,          // качество вывода
                              VARIABLE_PITCH,           // шаг между символами шрифта и семейство
                              TEXT("Times New Roman"));

	TC_ITEM tci;
	tci.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	tci.iImage = -1;
	wnd = CreateWindowEx(0x00000000, "SysTabControl32", "", WS_VISIBLE|WS_CHILD|WS_BORDER, 0, 0, 580, 500, parent, (HMENU) TAB0, instance, NULL);
        SetClassLong(wnd, GCL_HBRBACKGROUND, (LONG) CreateSolidBrush(RGB(255, 255, 255)));
        InvalidateRect(wnd, NULL, TRUE);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);

	tci.pszText = "Сканирование";
	tci.lParam = 5008;
	SendMessage(wnd, TCM_INSERTITEM, 0, (LPARAM) &tci);

	tci.pszText = "Шифрование";
	tci.lParam = 5009;
	SendMessage(wnd, TCM_INSERTITEM, 1, (LPARAM) &tci);

	tci.pszText = "Автозапуск";
	tci.lParam = 5010;
	SendMessage(wnd, TCM_INSERTITEM, 2, (LPARAM) &tci);

	tci.pszText = "Дополнительно";
	tci.lParam = 5011;
	SendMessage(wnd, TCM_INSERTITEM, 3, (LPARAM) &tci);
/*============================================================Сканирование==================================================================*/
/*==========================================================================================================================================*/
	wnd = CreateWindowEx(0x00000000, "Button", "Каталог", 0x50010009, 32, 135, 96, 32, parent, (HMENU) IDC_RADIO0, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Button", "Файл", 0x50010009, 32, 185, 96, 32, parent, (HMENU) IDC_RADIO1, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
    wnd = CreateWindowEx(0x00000000, "Button", "Выполнить сканирование", 0x50012F00, 194, 184, 352, 32, parent, (HMENU) IDC_BUTTON1, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Button", "Проверить целостность файлов", 0x50012F00, 38, 345, 504, 32, parent, (HMENU) IDC_BUTTON2, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Static", "Контроль  целостности  системы  (обнаружение  изменений)  заключается  в  про-", 0x50000300, 29, 265, 515, 15, parent, (HMENU) IDC_STATIC1, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
    wnd = CreateWindowEx(0x00000000, "Static", "верке  и  сравнении   текущих  параметров  вычислительной  системы  с   эталон-", 0x50000300, 29, 286, 515, 15, parent, (HMENU) IDC_STATIC6, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Static", "ными  параметрами,  соответствующими  ее  не  зараженному   состоянию.", 0x50000300, 29, 308, 516, 15, parent, (HMENU) IDC_STATIC7, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Button", "Выбрать...", 0x50012F00, 450, 144, 96, 24, parent, (HMENU) IDC_BUTTON0, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Button", "CRC-сканер", 0x50020007, 16, 240, 550, 150, parent, (HMENU) IDC_GROUP2, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "Укажите путь...",0x50010080 , 194, 144, 248, 24, parent, (HMENU) IDC_EDIT0, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Static", "Метод  сигнатурного  сканирования  заключается  в  выявлении  вредоносного  ПО",
                      0x50000300, 24, 65, 530, 15, parent, (HMENU) IDC_STATIC0, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
    wnd = CreateWindowEx(0x00000000, "Static", "по уникальному программному коду (сигнатуре). Обнаружение вирусов осуществля-",
                      0x50000300, 24, 87, 532, 15, parent, (HMENU) IDC_STATIC4, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
    wnd = CreateWindowEx(0x00000000, "Static", "ется  путем  сравнения  хэш-сумм  файлов  с  базой  сигнутур  вирусов.",
                      0x50000300, 24, 109, 532, 15, parent, (HMENU) IDC_STATIC5, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
	wnd = CreateWindowEx(0x00000000, "Button", "MD5-сканер", 0x50020007, 15, 40, 550, 190, parent, (HMENU) IDC_GROUP0, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5008);
/*==========================================================================================================================================*/

/*============================================================Шифрование====================================================================*/
/*==========================================================================================================================================*/
	wnd = CreateWindowEx(0x00000000, "Button", "Зашифровать", 0x40010009, 16, 274, 107, 32, parent, (HMENU) IDC_RADIO2, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
	wnd = CreateWindowEx(0x00000000, "Button", "Расшифровать", 0x40010009, 16, 337, 107, 32, parent, (HMENU) IDC_RADIO3, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
	wnd = CreateWindowEx(0x00000200, "Edit", "Укажите путь...", 0x40010080, 170, 275, 285, 25, parent, (HMENU) IDC_EDIT1, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
	wnd = CreateWindowEx(0x00000000, "Button", "Выполнить", 0x40010001, 170, 327, 395, 50, parent, (HMENU) IDC_BUTTON3, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
	wnd = CreateWindowEx(0x00000000, "Button", "Выбрать...", 0x40010001, 475, 275, 90, 25, parent, (HMENU) IDC_BUTTON4, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
    wnd = CreateWindowEx(0x00000000, "Static", "Укажите пароль:",0x40000300, 16, 235, 110, 15, parent, (HMENU) IDC_STATIC3, instance, NULL);
    SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
	wnd = CreateWindowEx(0x00000000, "Static", "Шифрование — это  способ  повышения  безопасности  файла, при котором его  содер-",
                      0x40000300, 16, 80, 544, 15, parent, (HMENU) IDC_STATIC2, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
    wnd = CreateWindowEx(0x00000000, "Static", "жимое  преобразуется  так,  что  оно  может  быть  прочитано  только  пользователем,",
                      0x40000300, 16, 100, 545, 15, parent, (HMENU) IDC_STATIC6, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
    wnd = CreateWindowEx(0x00000000, "Static", "обладающим соответствующим ключом шифрования для расшифровки содержимого.",
                      0x40000300, 16, 120, 544, 15, parent, (HMENU) IDC_STATIC7, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
    wnd = CreateWindowEx(0x00000000, "Static", "Используйте   шифрование,   когда   требуется   передать   файл   по   незащищённому",
                      0x40000300, 16, 160, 544, 15, parent, (HMENU) IDC_STATIC8, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
    wnd = CreateWindowEx(0x00000000, "Static", "каналу  связи.",
                      0x40000300, 16, 180, 544, 15, parent, (HMENU) IDC_STATIC9, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
	wnd = CreateWindowEx(0x00000000, "Button", "Шифрование", 0x40020007, 10, 40, 560, 176, parent, (HMENU) IDC_GROUP3, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
    wnd = CreateWindowEx(0x00000200, "Edit", "", 0x40010080|WS_BORDER|ES_PASSWORD, 170, 230, 393, 23, parent, (HMENU) IDC_EDIT2, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5009);
/*==========================================================================================================================================*/

/*============================================================Автозапуск====================================================================*/
/*==========================================================================================================================================*/
	wnd = CreateWindowEx(0x00000000, "Static", "Часто   заражение   компьютера   вирусом   происходит   посредством   функции   ав-",
                      0x40000300, 24, 76, 544, 15, parent, (HMENU) IDC_STATIC3, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5010);
    wnd = CreateWindowEx(0x00000000, "Static", "тозапуска   съёмных   носителей   информации.",
                      0x40000300, 24, 100, 544, 15, parent, (HMENU) IDC_STATIC10, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5010);
    wnd = CreateWindowEx(0x00000000, "Static", "Решением   проблемы   является   отключение   функции   автозапуска.",
                      0x40000300, 24, 160, 544, 15, parent, (HMENU) IDC_STATIC10, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5010);
//     wnd = CreateWindowEx(0x00000000, "Static", "Автозапуск помогает",
//                      0x40000300, 24, 56, 544, 152, parent, (HMENU) IDC_STATIC12, instance, NULL);
//        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
//        SetWindowLong(wnd, GWL_USERDATA, 5010);
	wnd = CreateWindowEx(0x00000000, "Button", "Включить автозапуск", 0x40010001, 16, 312, 552, 56, parent, (HMENU) IDC_BUTTON6, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5010);
	wnd = CreateWindowEx(0x00000000, "Button", "Отключить автозапуск", 0x40010001, 16, 232, 552, 56, parent, (HMENU) IDC_BUTTON5, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5010);
    wnd = CreateWindowEx(0x00000000, "Button", "Автозапуск", 0x40020007, 10, 32, 560, 184, parent, (HMENU) IDC_GROUP4, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5010);
/*==========================================================================================================================================*/

/*============================================================Дополнительно=================================================================*/
/*==========================================================================================================================================*/
wnd = CreateWindowEx(0x00000000, "Static", "Программа  «ПСЗИ»  предназначена   для   защиты   информации   на   компьютерах,",
                      0x40000300, 24, 75, 544, 15, parent, (HMENU) IDC_STATIC11, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);
    wnd = CreateWindowEx(0x00000000, "Static", "работающих   под   управлением   операционной   системы   Windows   (XP  и  выше).",
                      0x40000300, 24, 100, 544, 15, parent, (HMENU) IDC_STATIC12, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);
    wnd = CreateWindowEx(0x00000000, "Static", "«ПЗСИ»   способна   проводить  сигнатурное  сканирование, сканирование на основе",
                      0x40000300, 24, 125, 544, 15, parent, (HMENU) IDC_STATIC13, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);
 wnd = CreateWindowEx(0x00000000, "Static", "подсчёта  контрольных   сумм,  выполнять   шифрование/расшифрование   файлов  и",
                      0x40000300, 24, 150, 544, 15, parent, (HMENU) IDC_STATIC14, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);
 wnd = CreateWindowEx(0x00000000, "Static", "отключение/включение автозапуска съёмных носителей информации по требованию.",
                      0x40000300, 24, 175, 544, 15, parent, (HMENU) IDC_STATIC15, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

 wnd = CreateWindowEx(0x00000000, "Static", "Программа  «ПСЗИ»  является  бесплатной  для  коммерческого или корпоративного",
                      0x40000300, 24, 205, 544, 15, parent, (HMENU) IDC_STATIC16, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);
wnd = CreateWindowEx(0x00000000, "Static", "использования.",
                      0x40000300, 24, 230, 544, 15, parent, (HMENU) IDC_STATIC17, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

wnd = CreateWindowEx(0x00000000, "Static", "Связаться  с  разработчиком  программы  вы  можете  по:",
                      0x40000300, 24, 290, 544, 15, parent, (HMENU) IDC_STATIC18, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

        wnd = CreateWindowEx(0x00000000, "Static", "–  E-mail :  d e f u z e @ y a . r u",
                      0x40000300, 40, 315, 544, 20, parent, (HMENU) IDC_STATIC19, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

        wnd = CreateWindowEx(0x00000000, "Static", "–  Skype :  d e f u 3 e",
                      0x40000300, 40, 340, 544, 17, parent, (HMENU) IDC_STATIC20, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

wnd = CreateWindowEx(0x00000000, "Button", "Основные сведения о программе", 0x50020007, 16, 45, 550, 210, parent, (HMENU) IDC_GROUP5, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

wnd = CreateWindowEx(0x00000000, "Button", "Обратная связь", 0x50020007, 16, 265, 550, 110, parent, (HMENU) IDC_GROUP6, instance, NULL);
        SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
        SetWindowLong(wnd, GWL_USERDATA, 5011);

/*==========================================================================================================================================*/

	HWND hRadioButton, hEdit;
	hRadioButton = GetDlgItem(parent, IDC_RADIO0);
	SendMessage(hRadioButton,BM_SETCHECK,BST_CHECKED,0);
	hRadioButton = GetDlgItem(parent, IDC_RADIO2);
	SendMessage(hRadioButton,BM_SETCHECK,BST_CHECKED,0);

    hEdit = GetDlgItem(parent, IDC_EDIT2);
	TOOLINFO ti = {0};
    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = NULL;
    ti.hinst = (HINSTANCE)GetModuleHandle(NULL);
    ti.uId = 1;
    //ti.lpszText = "first";
    RECT frst = {20, 20, 70, 50};
    ti.rect.left = frst.left;
    ti.rect.top = frst.top;
    ti.rect.right = frst.right;
    ti.rect.bottom = frst.bottom;
    //добавляем подсказку для первого прямоугольничка
    SendMessage(hEdit, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}

/*=====================================================Регистрация класса окна=============================================================*/
/*=========================================================================================================================================*/
void register_classes()
{
	WNDCLASS wc;

	wc.cbClsExtra    = 0;                               /* дополнительная память */
	wc.cbWndExtra    = 0;                               /*  в байтах */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);          //дескриптор кисти для фона
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);     //дескриптор курсора приложения
	wc.hIcon         = (HICON)
                      LoadImage(                        // дескриптор значка
                      GetModuleHandle(NULL),
                      "IDI_ICON",
                      IMAGE_ICON,                   // тип загружаемого изображения
                      16,                           // ширина значка
                      16,                           // высота значка
                      0
                    );
	wc.hInstance     = instance;                    // дескриптор экземпляра приложения
	wc.lpszMenuName  = NULL;                        // указатель на имя класса окна
	wc.style         = CS_HREDRAW|                  // перерисовка окна при перемещении
                        CS_HREDRAW|
                        CS_BYTEALIGNCLIENT|     // выравнивание окна по x
                        CS_PARENTDC | !(WS_MAXIMIZEBOX);            // отсечение родительского окна (производительности)

	wc.lpfnWndProc   = wnd_proc0;
	wc.lpszClassName = WND_CLASS_NAME0;         // имя класса

	RegisterClass(&wc);                         // регистрация главного окна
}
/*=========================================================================================================================================*/

/*==================================================Цикл сообщений=========================================================================*/
/*=========================================================================================================================================*/
int message_loop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);         // перевод нажатых клавиш в читаемый вид
		DispatchMessage(&msg);          // Вызов обработчика
	}

	return msg.wParam;
}
/*=========================================================================================================================================*/
