// BaiscOCR.h

#pragma once

#include "ocropus.h"
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __UNIX__
#include <glob.h>
#endif
#include "colib.h"
#include "imglib/iulib.h"
#include "ocropus.h"
#include "glinerec.h"
#include "bookstore.h"
#include "linesegs.h"
#include "ocr-commands.h"

#include "CUtils.h"

using namespace System;
using namespace ocropus;
using namespace glinerec;
using namespace iulib;

namespace BasicOCR {

	public ref class BasicOCR
	{
		public:
			BasicOCR();
	};
}
