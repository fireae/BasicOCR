// CLine.h

#pragma once

#include "colib.h"

#include "CWord.h"
#include "CDocObject.h"
#include "CCharacter.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace colib;

namespace BasicOCR {

	[XmlRootAttribute("Line")]
	public ref class CLine : public CDocObject
	{
		public:
			CLine();
			CLine(rectangle obj_box);
			CLine(float documentHeight, rectangle obj_box, colib::nustring text);
			//~CLine();
			//[Category("Contents")]
			property String ^LineText;
			//[Browsable(false)]
			property List<CCharacter ^> ^Characters;
		private:

	};
}