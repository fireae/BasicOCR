#include "stdafx.h"

#include "CCharacter.h"

namespace BasicOCR 
{
	CCharacter::CCharacter() : CDocObject() { }
	CCharacter::CCharacter(rectangle obj_box) : CDocObject(obj_box) { }
	CCharacter::CCharacter(rectangle obj_box, float char_cost, int char_ascii) : CDocObject(obj_box) 
	{
		Confidence = char_cost;
		Cost = char_cost;
		Letter += (Value = Char((char)char_ascii));
	}
	CCharacter::CCharacter(float documentHeight, rectangle obj_box, float char_cost, int char_ascii) : CDocObject() 
	{
		X = obj_box.x0;
		Y = documentHeight - obj_box.y0 - obj_box.height();
		Width = obj_box.width();
		Height = obj_box.height();

		Confidence = char_cost;
		Cost = char_cost;
		Letter += (Value = Char((char)char_ascii));
	}
	CCharacter::CCharacter(rectangle obj_box, Double confidence, Char character, int fontindex, int pointsize, int formatting) : CDocObject(obj_box) 
	{
		/*
		X = -obj_box.x0;
		Y = obj_box.y0;
		Width = obj_box.width();
		Height = obj_box.height();
		*/

		Confidence = confidence;
		Cost = confidence;
		FontIndex = fontindex;
		PointSize = pointsize;
		Formatting = formatting;
		Letter += (Value = character);
	}
}