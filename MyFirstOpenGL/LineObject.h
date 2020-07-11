#pragma once
#include "DrawingObject.h"
class LineObject :
	public DrawingObject
{
public:
	LineObject() : m_x1(0), m_y1(0), m_x2(0), m_y2(0)
	{

	}

	void Draw();

private:
	float m_x1;
	float m_y1;
	float m_x2;
	float m_y2;
};

