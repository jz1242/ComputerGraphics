#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "rayCone.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCone::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	throw RayException( "RayCone::intersect undefined" );
	return -1;
}

BoundingBox3D RayCone::setBoundingBox( void )
{
	throw RayException( "RayCone::setBoundingBox undefined" );
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayCone::setUpOpenGL( int cplx , bool setBufferObjects )
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	throw RayException( "RayCone::setUpOpenGL undefined" );
}

int RayCone::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	throw RayException( "RayCone::drawOpenGL undefined" );
	return -1;
}
