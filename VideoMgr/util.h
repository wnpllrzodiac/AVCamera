
#ifndef _UTIL_H
#define _UTIL_H

#include "Stdafx.h"
#include <vcclr.h>

namespace VideoMgr
{
	//reference: http://msdn.microsoft.com/zh-cn/library/1b4az623.aspx
	static void String2string ( System::String^ s, std::string& os ) 
	{
		using namespace Runtime::InteropServices;
		const char* chars = 
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	static void String2wstring ( System::String^ s, std::wstring& os ) 
	{
		using namespace Runtime::InteropServices;
		const wchar_t* chars = 
			(const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

}
#endif
