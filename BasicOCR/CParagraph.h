// CParagraph.h

#pragma once

#include "colib/colib.h"

#include "CDocObject.h"
#include "CLine.h"

using namespace System;
using namespace colib;

namespace BasicOCR {

	[XmlRootAttribute("Paragraph")]
	public ref class CParagraph: public CDocObject
	{
		public:
			CParagraph();
			CParagraph(rectangle obj_box);
			CParagraph(float documentHeight, rectangle obj_box);
			bool Contains(CLine ^lineObj);
			//~CParagraph();
			//[Browsable(false)]
			property List<CLine ^> ^Lines;
			//[Category("Contents")]
			property String ^Text;
		private:

	};
}