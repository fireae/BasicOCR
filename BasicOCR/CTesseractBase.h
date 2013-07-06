#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "baseapi.h"

using namespace System;
using namespace System::Xml;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Xml::Serialization;
using namespace System::Collections::Generic;
//using namespace tesseract;

namespace BasicOCR
{
	public class TesseractBase : public TessBaseAPI
	{
	public:	
		TesseractBase();
		void TesseractBase::CopyImageToTesseract(const unsigned char* imagedata, int bytes_per_pixel, int bytes_per_line, int left, int top, int width, int height);
		void TesseractBase::CopyBinaryRect(const unsigned char* imagedata, int bytes_per_line, int left, int top, int width, int height);
		void TesseractBase::FindLines(BLOCK_LIST* block_list);
		PAGE_RES* TesseractBase::Recognize(BLOCK_LIST* block_list, struct ETEXT_STRUCT* monitor);

	};
}