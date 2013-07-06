// CParagraph.h

#pragma once

#include "colib.h"

using namespace System;
using namespace System::Xml::Serialization;
using namespace colib;

namespace BasicOCR {

	public ref class CDocObject
	{
		public:
			CDocObject();
			CDocObject(rectangle obj_box);
			[XmlAttributeAttribute("X")]
			property int X;
			[XmlAttributeAttribute("Y")]
			property int Y;
			[XmlAttributeAttribute("Width")]
			property int Width;
			[XmlAttributeAttribute("Height")]
			property int Height;

	};
}