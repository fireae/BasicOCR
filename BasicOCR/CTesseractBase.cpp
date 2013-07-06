#include "stdafx.h"
#include "CTesseractBase.h"

namespace BasicOCR
{
	TesseractBase::TesseractBase() : TessBaseAPI(){}

	void TesseractBase::CopyImageToTesseract(const unsigned char* imagedata, int bytes_per_pixel, int bytes_per_line, int left, int top, int width, int height)
	{
		TessBaseAPI::CopyImageToTesseract(imagedata, bytes_per_pixel, bytes_per_line, left, top, width, height);
	}

	void TesseractBase::CopyBinaryRect(const unsigned char* imagedata, int bytes_per_line, int left, int top, int width, int height)
	{
		TessBaseAPI::CopyBinaryRect(imagedata, bytes_per_line, left, top, width, height);
	}
	void TesseractBase::FindLines(BLOCK_LIST* block_list)
	{
		TessBaseAPI::FindLines(block_list);
	}

	PAGE_RES* TesseractBase::Recognize(BLOCK_LIST* block_list, struct ETEXT_STRUCT* monitor)
	{
		return TessBaseAPI::Recognize(block_list, monitor);
	}
}