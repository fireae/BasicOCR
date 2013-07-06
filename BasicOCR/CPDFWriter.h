#pragma once
#include <setjmp.h>
#include "hpdf.h"
#include "colib.h"

jmp_buf env;

#ifdef HPDF_DLL
void __stdcall
#else
void
#endif

cbHandleErr (HPDF_STATUS errNum, HPDF_STATUS detail_num, void *userinfo)
{
	printf ("ERROR: errNum=%04X, detail_num=%u\n", (HPDF_UINT)errNum,(HPDF_UINT)detail_num);
	longjmp(env, 1);
}

class PDFWriter
{
	HPDF_Doc objPDF;
	HPDF_Page pdfPage;
	HPDF_Font font;
	HPDF_REAL pageHeight;
public:	
	//void PDFWriter(){}
	//void ~PDFWriter(){}
	void initPDFWriter( HPDF_REAL pageWidth, HPDF_REAL pageHeight )
	{
		objPDF = HPDF_New ( (HPDF_Error_Handler)cbHandleErr, NULL );
		if ( !objPDF )
		{
			throw ( "Can't create PdfDoc object\n" );
		}

		if ( setjmp( env ) ) 
		{
			HPDF_Free ( objPDF );
			throw "An unknown error was thrown.";
		}

		pdfPage = HPDF_AddPage( objPDF );
		HPDF_Page_SetWidth( pdfPage, pageWidth );
		HPDF_Page_SetHeight( pdfPage, pageHeight );
		pageHeight = HPDF_Page_GetHeight( pdfPage );

		HPDF_Page_BeginText( pdfPage );
	}		
	//TODO gary: add font options, font detection? multipage? characterFontSize needs to be character dependent
	void writeLineToPDF( char *line, HPDF_REAL x, HPDF_REAL y, HPDF_REAL characterFontSize )
	{   
		font = HPDF_GetFont( objPDF, "Times-Roman", NULL );
		HPDF_Page_SetFontAndSize( pdfPage, font, characterFontSize );
		HPDF_Page_TextOut( pdfPage, x, y, line );
	}
	void savePDFToFile( char *FileName )
	{
		HPDF_Page_EndText( pdfPage );
		HPDF_SaveToFile( objPDF, FileName );
		HPDF_Free( objPDF );
	}
};