// CWord.h

#pragma once

#include "colib.h"
#include "CCharacter.h"
#include "CDocObject.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace colib;

namespace BasicOCR {

	[XmlRootAttribute("Word")]
	public ref class CWord : public CDocObject
	{
		public:
		CWord();
		CWord(rectangle obj_box);
		//~CWord();
		
		int LineIndex;
		int Blanks;
		int FontIndex, PointSize, Formating;
		int Left, Right, Top, Bottom;
		
		String^ Text;
		Double Confidence;
		Object^ Tag;
		List<CCharacter ^>^ CharList;

		virtual String^ ToString() override
		{
			return String::Format("{0} ({1})", Text, Confidence);
		}

		private:

	};
}