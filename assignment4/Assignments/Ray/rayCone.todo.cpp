#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <Util/exceptions.h>
#include "rayScene.h"
#include "rayCone.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCone::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	Util::Throw( "RayCone::intersect undefined" );
	return -1;
}

BoundingBox3D RayCone::setBoundingBox( void )
{
	Util::Throw( "RayCone::setBoundingBox undefined" );
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayCone::setUpOpenGL( int cplx , bool setBufferObjects )
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	Util::Throw( "RayCone::setUpOpenGL undefined" );
}

int RayCone::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	Util::Throw( "RayCone::drawOpenGL undefined" );
	return -1;
}
