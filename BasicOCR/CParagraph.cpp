#include "stdafx.h"

#include "CParagraph.h"

namespace BasicOCR {

	CParagraph::CParagraph() : CDocObject() { }

	CParagraph::CParagraph(rectangle obj_box) : CDocObject(obj_box) {
		Lines = gcnew List<CLine ^>();
	}

	CParagraph::CParagraph(float documentHeight, rectangle obj_box) : CDocObject() {
		X = obj_box.x0;
		Y = documentHeight - obj_box.y0 - obj_box.height();
		Width = obj_box.width();
		Height = obj_box.height();
		Lines = gcnew List<CLine ^>();
	}

	bool CParagraph::Contains(CLine ^lineObj) {
		if (lineObj->X >= X && lineObj->Width <= Width &&
			lineObj->Y >= Y && lineObj->Height <= Height)
			return true;
		return false;
	}

}