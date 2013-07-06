// CUtils.h

#pragma once

#include "vcclr.h"
#include "narray.h"

//using namespace std;
using namespace System;

namespace BasicOCR {

	static char *c_str(String^ input) {
		// Convert to a char*
		pin_ptr<const wchar_t> wch = PtrToStringChars(input);
		size_t origsize = wcslen(wch) + 1;
		size_t convertedChars = 0;
		char * nstring;
		nstring = new char[origsize];
		wcstombs_s(&convertedChars, nstring, origsize, wch, _TRUNCATE);
		return nstring;
	}

/*	static array<int> ^managed_intarray(intarray to_convert) {
		array<int> ^converted = gcnew array<int>(to_convert.length());
		for(int i = 0; i < to_convert.length(); i++) {
			converted[i] = to_convert[i];
		}
		return converted;
	}

	static intarray native_intarray(array<int> ^to_convert) {
		intarray converted(1);
		converted.resize(to_convert->Length);
		for(int i = 0; i < to_convert->Length; i++) {
			converted[i] = to_convert[i];
		}
		return converted;
	}*/

}
