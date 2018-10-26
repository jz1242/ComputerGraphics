#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "rayBox.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	throw RayException( "RayBox::intersect undefined" );
	return -1;
}
BoundingBox3D RayBox::setBoundingBox( void )
{
	//throw RayException( "RayBox::setBoundingBox undefined" );
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayBox::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	throw RayException( "RayBox::drawOpenGL undefined" );
	return -1;
}
