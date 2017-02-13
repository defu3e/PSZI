#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <windows.h>

#define U8       uint8_t

extern  void    errors_list (U8 ecode, char *param);    // функция обработки ошибок
