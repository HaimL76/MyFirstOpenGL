#include <sstream>
#include <math.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "MyTime.h"
#include <time.h>
#include <Windowsx.h>

#include "LineObject.h"

void LineObject::Draw()
{
    glBegin(GL_LINES);

	int length = 1;
	int angle = 3;

	m_x2 = m_x1 + length * cos(angle);
	m_y2 = m_y1 + length * sin(angle);

	glVertex2f(m_x1, m_y1);
	glVertex2f(m_x2, m_y2);

	glEnd();
}
