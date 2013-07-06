// This is the main DLL file.
#include "stdafx.h"
#include "CDocument.h"	

extern ESHM_INFO shm;

namespace BasicOCR 
{
	CDocument::CDocument() 
	{
		mBasicOCRTessBaseAPIInstance = NULL;
		mpageRes = NULL;
		Words = gcnew List<CWord^>;
		InitOcropus();
	};

	CDocument::~CDocument()
	{
		TesseractClear();
	}

	void CDocument::SetImagePath(String ^ImageFilePath) 
	{
		ImagePath = ImageFilePath;
	}

	CDocument::CDocument(String ^ImageFilePath) 
	{
		ImagePath = ImageFilePath;
		InitOcropus();
	}

	void CDocument::InitOcropus() 
	{
		Paragraphs = gcnew List<CParagraph ^>();
		Columns = gcnew List<CColumn ^>();
		Lines = gcnew List<CLine ^>();
		Words = gcnew List<CWord ^>();
		Characters = gcnew List<CCharacter ^>();
		init_ocropus_components();
		init_glclass();
		init_glfmaps();
		init_linerec();
	}

//******************************
// Imaging
//******************************

	System::Drawing::Bitmap^ CDocument::GetThresholdedImage(System::Drawing::Bitmap^ bitmap, System::Drawing::Rectangle rect)
	{
		NewImage(bitmap, rect);

		int width = page_image.get_xsize();
		int height = page_image.get_ysize();
		
		System::Drawing::Bitmap^ result = gcnew Bitmap(width, height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		Imaging::BitmapData ^data = result->LockBits(System::Drawing::Rectangle(0, 0, width, height), Imaging::ImageLockMode::ReadWrite, result->PixelFormat);
		
		BYTE *scan0 = (BYTE *)data->Scan0.ToPointer();
		
		IMAGELINE line;

		for (int y = 0; y < height; y++)
		{
			page_image.get_line(0, y, width, &line, 0);
			BYTE *dest = scan0 + data->Stride*(height - y - 1);
			BYTE *src = (BYTE *)line.pixels;
			for (int x = 0; x < width; x++)
			{
				BYTE val = (*src++)*255;
				*dest++ = val;
				*dest++ = val;
				*dest++ = val;
			}
		}
		
		result->UnlockBits(data);
		
		return result;
	}

	void CDocument::NewImage(System::Drawing::Bitmap^ bitmap, System::Drawing::Rectangle rect)
	{
		if (rect == System::Drawing::Rectangle::Empty)
			rect = System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height);

		documentHeight = bitmap->Height;
		documentWidth = bitmap->Width;

		System::Drawing::Bitmap^ tmp = bitmap;

		if (System::Drawing::Image::IsAlphaPixelFormat(bitmap->PixelFormat))
		{
			tmp = gcnew System::Drawing::Bitmap(bitmap->Width, bitmap->Height, Imaging::PixelFormat::Format24bppRgb);
		
			tmp->SetResolution(bitmap->HorizontalResolution, bitmap->VerticalResolution);

			System::Drawing::Graphics^ gr = System::Drawing::Graphics::FromImage(tmp);

			gr->DrawImageUnscaled(bitmap, 0, 0);

			delete gr;
		}

		int bpp=-1;

		if (tmp->PixelFormat == Imaging::PixelFormat::Format1bppIndexed)
			bpp = 0;
		else if (tmp->PixelFormat == Imaging::PixelFormat::Format8bppIndexed)
			bpp = 1;
		else if (tmp->PixelFormat == Imaging::PixelFormat::Format24bppRgb)
			bpp = 3;

		if (bpp>-1)
		{
			Imaging::BitmapData ^data = tmp->LockBits(rect, Imaging::ImageLockMode::ReadWrite, tmp->PixelFormat);
			
			BYTE *scan0 = NULL;
			int stride = data->Stride;
			if (data->Stride > 0)
				scan0 = (BYTE *)data->Scan0.ToPointer();
			else
			{
				stride = -data->Stride;

				scan0 = new BYTE[data->Height * stride];

				BYTE *scan0b = (BYTE *)data->Scan0.ToPointer();
				for (int y = 0; y < data->Height; y++)
					memcpy(scan0 + y * stride, scan0b - y * stride, stride);
			}
			if (bpp == 0)
				mBasicOCRTessBaseAPIInstance->CopyBinaryRect(scan0, stride, 0, 0, rect.Width, rect.Height);
			else
				mBasicOCRTessBaseAPIInstance->CopyImageToTesseract(scan0, bpp, stride, 0, 0, rect.Width, rect.Height);
			
			if (data->Stride <= 0)
				delete[] scan0;
			
			tmp->UnlockBits(data);
		}

		if (tmp != bitmap)
			delete tmp;
	}

//**********************************
// Tesseract
//**********************************

	int CDocument::TesseractInit(String^ tessdata, String^ lang, bool numericMode)
	{
		TesseractClear();

		mBasicOCRTessBaseAPIInstance = new TesseractBase();

		int result = mBasicOCRTessBaseAPIInstance->InitWithLanguage(c_str(tessdata),
																	NULL, c_str(lang), 
																	NULL, numericMode, 
																	0, NULL);
		
		SetVariable("tessedit_write_ratings", true);
		SetVariable("tessedit_zero_rejection", true);
		
		return result;
	}

	void CDocument::TesseractClear()
	{
		
	page_image.destroy();
	if (mpageRes)
		delete mpageRes;
	mpageRes = NULL;
		if (mBasicOCRTessBaseAPIInstance)
		{
			mBasicOCRTessBaseAPIInstance->End();
			delete mBasicOCRTessBaseAPIInstance;
		}
		mBasicOCRTessBaseAPIInstance = NULL;
	}

	bool CDocument::SetVariable(String^ name, Object^ val)
	{
		INT_VARIABLE_C_IT int_it(INT_VARIABLE::get_head());
		BOOL_VARIABLE_C_IT bool_it(BOOL_VARIABLE::get_head());
		STRING_VARIABLE_C_IT str_it(STRING_VARIABLE::get_head());
		double_VARIABLE_C_IT dbl_it(double_VARIABLE::get_head());

		bool bFound = false;

		for (int_it.mark_cycle_pt(); !int_it.cycled_list(); int_it.forward()) 
		{
			if (!strcmp(int_it.data()->name_str(), c_str(name)))
			{
				int_it.data()->set_value(Convert::ToInt32(val));
				bFound=true;
				break;
			}
		}
		
		if (!bFound)
		{
			for (bool_it.mark_cycle_pt(); !bool_it.cycled_list(); bool_it.forward()) 
			{
				if (!strcmp(bool_it.data()->name_str(), c_str(name)))
				{
					bool_it.data()->set_value(Convert::ToByte(val));
					bFound=true;
					break;
				}
			}
		}

		if (!bFound)
		{
			for (str_it.mark_cycle_pt(); !str_it.cycled_list(); str_it.forward()) 
			{
				if (!strcmp(str_it.data()->name_str(), c_str(name)))
				{
					str_it.data()->set_value(STRING(c_str(name)));
					bFound=true;
					break;
				}
			}
		}

		if (!bFound)
		{
			for (dbl_it.mark_cycle_pt(); !dbl_it.cycled_list(); dbl_it.forward()) 
			{
				if (!strcmp(dbl_it.data()->name_str(), c_str(name)))
				{
					dbl_it.data()->set_value(Convert::ToDouble(val));
					bFound=true;
					break;
				}
			}
		}

		return bFound;
	}
	Object^ CDocument::GetVariable (String^ name)
	{
		Object^ result = nullptr;

		INT_VARIABLE_C_IT int_it(INT_VARIABLE::get_head());
		BOOL_VARIABLE_C_IT bool_it(BOOL_VARIABLE::get_head());
		STRING_VARIABLE_C_IT str_it(STRING_VARIABLE::get_head());
		double_VARIABLE_C_IT dbl_it(double_VARIABLE::get_head());

		for (int_it.mark_cycle_pt(); !int_it.cycled_list(); int_it.forward()) 
		{
			if (!strcmp(int_it.data()->name_str(), c_str(name)/*__name*/))
			{
				int tmp = *int_it.data();
				result = tmp;
				break;
			}
		}
		
		if (result==nullptr)
		{
			for (bool_it.mark_cycle_pt(); !bool_it.cycled_list(); bool_it.forward()) 
			{
				if (!strcmp(bool_it.data()->name_str(), c_str(name)))
				{
					System::Byte tmp = *bool_it.data();
					result = tmp;
					break;
				}
			}
		}

		if (result==nullptr)
		{
			for (str_it.mark_cycle_pt(); !str_it.cycled_list(); str_it.forward()) 
			{
				if (!strcmp(str_it.data()->name_str(), c_str(name)))
				{
					STRING tmp = *str_it.data();
					result = gcnew String(tmp.string());
					break;
				}
			}
		}

		if (result==nullptr)
		{
			for (dbl_it.mark_cycle_pt(); !dbl_it.cycled_list(); dbl_it.forward()) 
			{
				if (!strcmp(dbl_it.data()->name_str(), c_str(name)))
				{
					double tmp = *dbl_it.data();
					result = tmp;
					break;
				}
			}
		}

		return result;
	}

	List<CWord ^>^ CDocument::TesseractRecognize(System::Drawing::Bitmap^ bitmap, System::Drawing::Rectangle rect)
	{
		init_metrics();
		set_tess_tweak_vars();

		init_baseline();
		init_bestfirst_vars();
		init_splitter_vars();
		init_associate_vars();
		init_chop();
		init_textord_vars();
		init_permute_vars();

		//init_dj_debug();
		InitAdaptiveClassifierVars(); 
		InitMFOutlineVars(); 
		InitNormProtoVars(); 
		InitIntProtoVars(); 
		InitIntegerMatcherVars(); 
		InitSpeckleVars(); 
		InitStopperVars(); 

		program_variables();
		mfeature_variables();
		program_init();
		mfeature_init();
		init_permute();
		//setup_cp_maps();

		List<CWord ^>^ result = nullptr;

		NewImage(bitmap, rect);

		int s1 = sizeof(ETEXT_DESC);
		int s2 = sizeof(EANYCODE_CHAR);
		int s3 = s1 + 32000 * s2;

		//shm.shm_size = 
		shmsetsizeandallocate(s1 + 32000 * s2);
		//shm.shm_mem = new BYTE[s3];//new BYTE[shm.shm_size];
		ZeroMemory(shm.shm_mem, shm.shm_size);
		mMonitor = ocr_setup_monitor();

		// Now run the main recognition.
		if (OcrDone==nullptr)
		{
			doOCR();
			result = BuildPage();
			free_variables();
		}
		else
		{
			mCallbackThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &CDocument::doCallback));
			mCallbackThread->Start();
		}

		return result;
	}

	void CDocument::doOCR()
	{

		BLOCK_LIST block_list;

		mBasicOCRTessBaseAPIInstance->FindLines(&block_list);

		mpageRes = mBasicOCRTessBaseAPIInstance->Recognize(&block_list, mMonitor);

		PAGE_RES_IT res_it(mpageRes);

		delete[] shm.shm_mem;
	}
	//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/fs/i_o_completion_ports.asp
	void CDocument::doCallback()
	{
		mOcrThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &CDocument::doOCR));
		mOcrThread->Priority = System::Threading::ThreadPriority::Normal;
		mOcrThread->Start();
		while (!mOcrThread->Join(100))
			if(mMonitor)
				ProgressEvent(mMonitor->progress);

		OcrDone(BuildPage());
		free_variables();
	}

	List<CWord^>^ CDocument::BuildPage()
	{
		CWord^ currentWord = nullptr;
		int j=0;
		int lineIndex=0;
		char unistr[8] = {};
		int confidenceTotal;
		int confidenceCount;

		for (int i = 0; i < mMonitor->count; i = j)
		{
			EANYCODE_CHAR* ch = &mMonitor->text[i];
			Characters->Add(gcnew CCharacter(colib::rectangle(ch->left, ch->top, ch->right, ch->bottom), 
							ch->confidence, ch->char_code, ch->font_index, ch->point_size, ch->formatting));
			if (currentWord == nullptr || ch->blanks > 0 || (currentWord != nullptr && (ch->left < currentWord->Left)))
			{
				if (currentWord != nullptr && ch->left < currentWord->Left)
						lineIndex++;
				currentWord = gcnew CWord();
				currentWord->CharList = gcnew List<CCharacter ^>();
				confidenceTotal = 0;
				confidenceCount = 0;
				currentWord->LineIndex = lineIndex;
				currentWord->Blanks = ch->blanks;
				currentWord->Left = ch->left;
				currentWord->Top = ch->top;
				currentWord->Right = ch->right;
				currentWord->Bottom = ch->bottom;
				currentWord->Width = ch->right;
				currentWord->Height = ch->bottom;
				for (j = i; j < mMonitor->count; j++)
				{
					EANYCODE_CHAR* unich = &mMonitor->text[j];
					if (ch->left != unich->left || ch->right != unich->right ||	ch->top != unich->top || ch->bottom != unich->bottom)
						break;
					unistr[j - i] = static_cast<unsigned char>(unich->char_code);
					// confidenceTotal += (int)((-unich->confidence * 0.035) * 5.0 + 100);
					confidenceTotal += unich->confidence; 
					confidenceCount++;

				}
				unistr[j - i] = 0;
				String^ tmp = gcnew String(unistr, 0, j - i, System::Text::Encoding::UTF8);
				currentWord->CharList->Add(gcnew CCharacter(colib::rectangle(ch->left, ch->top, ch->right, ch->bottom),ch->confidence, 
															ch->char_code, ch->font_index, ch->point_size, ch->formatting));
				currentWord->Text = (String^)tmp;
				currentWord->Confidence = (Double)confidenceTotal / (Double)confidenceCount;
				currentWord->FontIndex = ch->font_index;
				currentWord->PointSize = ch->point_size;
				currentWord->Formating = ch->formatting;
				Words->Add(currentWord);
			}
			else
			{
				if (currentWord != nullptr)
				{
					for (j = i; j < mMonitor->count; j++)
					{

						EANYCODE_CHAR* unich = &mMonitor->text[j];

						if (ch->left != unich->left || ch->right != unich->right ||	ch->top != unich->top || ch->bottom != unich->bottom)
							break;
						
						unistr[j - i] = static_cast<unsigned char>(unich->char_code);
						// confidenceTotal += (int)((-unich->confidence*0.035)*5.0+100);
						confidenceTotal += unich->confidence; // Every characters of a word have the same value, useless but I leave it
						confidenceCount++;
					}
					
					unistr[j - i] = 0;
					
					String^ tmp = gcnew String(unistr, 0, j-i, System::Text::Encoding::UTF8);
				
					currentWord->CharList->Add(gcnew CCharacter(colib::rectangle(ch->left, ch->top, ch->right, ch->bottom),tmp[0], ch->confidence));
					currentWord->Text += tmp;
					currentWord->Confidence = (Double)confidenceTotal / (Double)confidenceCount;
					currentWord->Left = Math::Min(currentWord->Left, (int)ch->left);
					currentWord->Top = Math::Min(currentWord->Top, (int)ch->top);
					currentWord->Width = currentWord->Right = Math::Max(currentWord->Right, (int)ch->right);
					currentWord->Height = currentWord->Bottom = Math::Max(currentWord->Bottom, (int)ch->bottom);
					
				}
				else
					j++;
			}
		}
		return Words;
	}
//*************************
// OCRopus
//*************************

	void CDocument::doOCRopusLayoutAnalysis()
	{
		param_bool recolor("recolor",0,"recolor segmentation");  //Gary - Soon to be OCRopus layout analysis
        param_string csegmenter("psegmenter","SegmentPageByRAST","segmenter to use at the page level");
        bytearray image;
        intarray segmentation;
        read_image_gray(image, c_str(ImagePath));

        autodel<ISegmentPage> segmenter;
        make_component(csegmenter,segmenter);
        segmenter->segment(segmentation,image);

		colib::narray<colib::nustring> text;
		colib::narray<colib::narray<colib::rectangle> > bboxes;
		colib::narray<colib::floatarray> costs;
		RecognizedPage result;
		
		int s1 = sizeof(ETEXT_DESC);
		int s2 = sizeof(EANYCODE_CHAR);
		int s3 = s1 + 32000 * s2;

		shmsetsizeandallocate(s1 + 32000 * s2);
		ZeroMemory(shm.shm_mem, shm.shm_size);
		mMonitor = ocr_setup_monitor();
		
		tesseract_recognize_blockwise("..\\", result, image, segmentation, mMonitor);
		tesseract_end();
		//extract paragraphs
		RegionExtractor regions;
		regions.setPageParagraphs(segmentation);
		
		documentWidth = regions.bbox(0).width();
		documentHeight = regions.bbox(0).height();
		for(int i = 0, len = regions.length(); i < len; i++) {
			Paragraphs->Add(gcnew CParagraph(documentHeight, regions.bbox(i)));
        }

		//extract columns
		regions.setPageColumns(segmentation);

		for(int i = 0, len = regions.length(); i < len; i++) {
			Columns->Add(gcnew CColumn(documentHeight, regions.bbox(i)));
		}

		CWord^ word = gcnew CWord;
		word->CharList = gcnew List<CCharacter^>;
		//extract lines, characters and text
		for(int i = 0, len = result.linesCount(); i < len; i++) {
			CLine ^clToAdd = gcnew CLine(documentHeight, result.bbox(i), (colib::nustring)result.getLineTexts(i) );
			for(int j = 0, len2 = result.getLineBboxes(i).length(); j < len2; j++) {
				CCharacter ^ccToAdd = gcnew CCharacter( documentHeight, result.getLineBboxes(i)(j), 
														result.getLineCosts(i)(j),
														result.getLineTexts(i)(j).ord());
				
				if(ccToAdd->Letter->ToString() == " ") {
					Words->Add(word);
					word->CharList->Clear();
				}
					
				word->CharList->Add(ccToAdd);
				Characters->Add(ccToAdd);
			}
			Lines->Add(clToAdd);
		}

		OcrDone(Words);

		delete[] shm.shm_mem;	
	}
	

	void CDocument::doOCRopusCallback() {
		mOcrThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &CDocument::doOCRopusLayoutAnalysis));
		mOcrThread->Priority = System::Threading::ThreadPriority::Normal;
		mOcrThread->Start();

		//fadein progress bar
		while (!mOcrThread->Join(100))
			if(mMonitor)
				ProgressEvent(mMonitor->progress);
		//fade out progress bar
	}

	void CDocument::OCRopusLayoutAnalysis() {
		mCallbackThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &CDocument::doOCRopusCallback));
		mCallbackThread->Start();
	}
//*************************
// Data Collection
//*************************
	int	CDocument::LineCount(List<CWord ^>^ words) {
		Dictionary<int, int>^ lines = gcnew Dictionary<int, int>();
		for each (CWord^ word in words)
		{
			if (!lines->ContainsKey(word->LineIndex))
				lines->Add(word->LineIndex, 1);
		}
		return lines->Count;
	}
	List<CWord ^>^ CDocument::GetLineWords(List<CWord ^>^ words, int lineIndex) {
		List<CWord ^>^ result = gcnew List<CWord ^>();
		for each (CWord^ word in words)
		{
			if (word->LineIndex==lineIndex)
				result->Add(word);
		}
		return result;
	}


	String^ CDocument::GetLineText (List<CWord ^>^ words, int lineIndex)
	{
		List<CWord ^>^ line = GetLineWords(words, lineIndex);
		System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
		String^ sep = String::Empty;
		for each (CWord^ word in line)
		{
			sb->Append(sep);
			sb->Append(word->Text);
			sep = " ";
		}
		return sb->ToString();
	}

//*********************************************
//	Document
//*********************************************
	void CDocument::Save(String ^FileName) {
		try 
		{
			XmlSerializer ^serializer = gcnew XmlSerializer(CDocument::typeid);
			TextWriter ^writer = gcnew StreamWriter(FileName);
			serializer->Serialize(writer, this);
			writer->Close();
		} catch (System::Exception ^ex) {}
	}

	void CDocument::Export( String ^FileName ) {//ugg reverse
		PDFWriter pdfWriter;

		pdfWriter.initPDFWriter( documentWidth, documentHeight );

		for each(CLine ^Line in Lines)
			pdfWriter.writeLineToPDF(c_str( Line->LineText ), Line->X, Line->Y, Line->Height);

		pdfWriter.savePDFToFile(c_str(FileName->ToString()));
	}

	void CDocument::Segment() {
		//segment page
		param_bool recolor("recolor",0,"recolor segmentation");
        param_string csegmenter("psegmenter","SegmentPageByRAST","segmenter to use at the page level");
        bytearray image;
        intarray segmentation;
        read_image_gray(image, c_str(ImagePath));
        autodel<ISegmentPage> segmenter;
        make_component(csegmenter,segmenter);
        segmenter->segment(segmentation,image);
        /*if(recolor) {
            make_page_segmentation_black(segmentation);
            simple_recolor(segmentation);
        }*/
		
		//extract paragraphs
		RegionExtractor regions;
		regions.setPageParagraphs(segmentation);
        for(int i = 0, len = regions.length(); i < len; i++) {
			Paragraphs->Add(gcnew CParagraph(regions.bbox(i)));
        }

		//extract columns
		regions.setPageColumns(segmentation);
		for(int i = 0, len = regions.length(); i < len; i++) {
			Columns->Add(gcnew CColumn(regions.bbox(i)));
		}

		//extract lines
		regions.setPageLines(segmentation);
		for(int i = 0, len = regions.length(); i < len; i++) {
			Lines->Add(gcnew CLine(regions.bbox(i)));
		}
		
	}

}