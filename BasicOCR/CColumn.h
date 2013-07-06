// CColumn.h

#pragma once

#include "colib/colib.h"

#include "CDocObject.h"
#include "CParagraph.h"

using namespace System;
using namespace colib;

namespace BasicOCR {

	[XmlRootAttribute("Column")]
	public ref class CColumn: public CDocObject
	{
		public:
			CColumn();
			CColumn(rectangle obj_box);
			CColumn(float documentHeight, rectangle obj_box);
			bool Contains(CParagraph ^paragraphObj);
			//~CColumn();
			//[Browsable(false)]
			property List<CParagraph ^> ^Paragraphs;

		private:

	};
}