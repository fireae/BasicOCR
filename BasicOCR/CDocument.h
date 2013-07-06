// CDocument.h
#pragma once

#include <windows.h>
#include <excpt.h>
#include <WinDef.h>

#include "applybox.h"
#include "control.h"
#include "tessvars.h"
#include "tessedit.h"
#include "baseapi.h"
//#include "thresholder.h"
#include "pageres.h"
#include "image/imgs.h"
#include "varabled.h"
#include "tprintf.h"
#include "tesseractmain.h"
#include "stderr.h"
#include "notdll.h"
#include "mainblk.h"
#include "output.h"
#include "globals.h"
//#include "helpers.h"
#include "blread.h"
#include "tfacep.h"
#include "callnet.h"
#include "baseline.h"
#include "djmenus.h"
#include "mfoutline.h"
#include "normmatch.h"
#include "intmatcher.h"
#include "speckle.h"
#include "tessinit.h"
#include "mfvars.h"




#include "ocropus.h"
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __UNIX__
#include <glob.h>
#endif
#include "colib.h"
#include "imglib/iulib.h"
#include "glinerec.h"
#include "bookstore.h"
#include "linesegs.h"
#include "ocr-commands.h"
#include "ocr-tesseract.h"

#include "CTesseractBase.h"

#include "CUtils.h"
#include "CParagraph.h"
#include "CColumn.h"
#include "CLine.h"
#include "CCharacter.h"
#include "CPDFWriter.h"
#include "CWord.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Threading;
using namespace ocropus;
using namespace glinerec;
using namespace colib;
//using namespace tesseract;

namespace BasicOCR {

	public ref class CDocument
	{
		public:
			CDocument();
			CDocument(String ^ImageFilePath);
			~CDocument();
			
			void SetImagePath(String ^ImageFilePath);
			void Segment();
			void Save(String ^FileName);
			void Export(String ^FileName);
			void doOCRopusLayoutAnalysis();
			void doOCRopusCallback();
			void OCRopusLayoutAnalysis();
			List<CWord ^>^ TesseractRecognize(System::Drawing::Bitmap^ bitmap, System::Drawing::Rectangle rect);
			
			//Tesseract
			int	TesseractInit(String^ tessdata, String^ lang, bool numericMode);
			void TesseractClear(); 
			delegate void ProgressHandler(int percent);
			delegate void OcrDoneHandler(List<CWord ^>^ words);
			bool SetVariable(String^ name, Object^ val);
			
			//Imaging
			Object^	GetVariable(String^ name);
			System::Drawing::Bitmap^ GetThresholdedImage(System::Drawing::Bitmap^ bitmap, System::Drawing::Rectangle rect);
			
			//Extracted Data
			static int LineCount(List<CWord ^>^ words);
			static List<CWord ^>^ GetLineWords(List<CWord ^>^ words, int lineIndex);
			static String^ GetLineText(List<CWord ^>^ words, int lineIndex);

			BLOCK_LIST* mBlockList;
			PAGE_RES* mpageRes;
			property String ^ImagePath;
			property List<CParagraph ^> ^Paragraphs;
			property List<CColumn ^> ^Columns;
			property List<CLine ^> ^Lines;
			property List<CWord ^> ^Words;
			property List<CCharacter ^> ^Characters;
			property float documentWidth;
			property float documentHeight;
			event ProgressHandler^ ProgressEvent;

			TesseractBase *mBasicOCRTessBaseAPIInstance;
			//PDFWriter *mPDFWriter;
			OcrDoneHandler^ OcrDone;
			
			static CDocument ^Load(String ^FileName) 
			{
				try {
					TextReader ^fileStream = gcnew StreamReader(FileName);
					XmlSerializer ^serializer = gcnew XmlSerializer(CDocument::typeid);
					return (CDocument ^)serializer->Deserialize(fileStream);
					fileStream->Close();
				} catch (System::Exception ^ex) {

				}
			}

		private:
			void InitOcropus();

			ETEXT_DESC* mMonitor;
			//PAGE_RES *mpageRes;

			void doOCR();
			void doCallback();
			void NewImage(System::Drawing::Bitmap^ bitmap, System::Drawing::Rectangle rect);
			Thread^ mOcrThread;
			Thread^ mCallbackThread;
			List<CWord^>^ BuildPage();
	};

}
