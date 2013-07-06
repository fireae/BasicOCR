#include "stdafx.h"

#include "CLine.h"

namespace BasicOCR {

	CLine::CLine() : CDocObject() { }

	CLine::CLine(rectangle obj_box) : CDocObject(obj_box) { }

	CLine::CLine(float documentHeight, rectangle obj_box, colib::nustring text) : CDocObject() {
		X = obj_box.x0;
		Y = documentHeight - obj_box.y0 - obj_box.height();
		Width = obj_box.width();
		Height = obj_box.height();
		for(int i = 0, len = text.length(); i < len; i++) {
			LineText += Char::ConvertFromUtf32(text(i).ord());
		}
		Characters = gcnew List<CCharacter ^>();
	}

}