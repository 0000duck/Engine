#pragma once
#include "Globals.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[80192];
	static char tmp_string2[80192];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 80192, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 80192, "\n%s(%d) : %s\n", file, line, tmp_string);
	OutputDebugString(tmp_string2);
}