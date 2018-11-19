#include "mouse.h"
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <Util/geometry.h>

Mouse::Mouse( void ) : startX(-1) , endX(-1) , startY(-1) , endY(-1) , scrollDown(false) , leftDown(false) , middleDown(false) , rightDown(false) , shiftDown(false) , ctrlDown(false) , altDown(false) {}

void Mouse::update( int button , int state , int x , int y )
{
	if( state==GLUT_DOWN )
	{
		startX = endX = x;
		startY = endY = y;
		leftDown   |= button==GLUT_LEFT_BUTTON;
		middleDown |= button==GLUT_MIDDLE_BUTTON;
		rightDown  |= button==GLUT_RIGHT_BUTTON;
		
		int c = glutGetModifiers();
		shiftDown = (c&GLUT_ACTIVE_SHIFT) != 0;
		ctrlDown  = (c&GLUT_ACTIVE_CTRL ) != 0;
		altDown   = (c&GLUT_ACTIVE_ALT  ) != 0;
	}
	else if( state==GLUT_UP )
	{
		endX = x;
		endY = y;
		leftDown   &= button!=GLUT_LEFT_BUTTON;
		middleDown &= button!=GLUT_MIDDLE_BUTTON;
		rightDown  &= button!=GLUT_RIGHT_BUTTON;
	}
}
Point2D Mouse::move( int x , int y )
{
	Point2D d( x-endX , y-endY );
	endX = x;
	endY = y;
	return d;
}