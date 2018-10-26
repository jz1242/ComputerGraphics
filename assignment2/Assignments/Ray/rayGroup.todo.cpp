#include <stdlib.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayGroup.h"
#include "rayTriangle.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	double dist = -1;
	int len = shapes.size();
	for (int i = 0; i < len; i++) {
		double val = shapes[i]->intersect(ray, iInfo, mx);
		if (val) {
			dist = val;
		}
	}
	return dist;
}

BoundingBox3D RayGroup::setBoundingBox( void )
{
	return bBox;
}

bool StaticRayGroup::set( void )
{
	static bool firstTime = true;
	if( firstTime ) fprintf( stderr , "[WARNING] StaticRayGroup::set undefined\n" );
	firstTime = false;
	return true;
}
//////////////////
// OpenGL stuff //
//////////////////
int RayGroup::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	throw RayException( "RayGroup::drawOpenGL undefined" );
	return -1;
}

//////////////////////////
// TriangleListRayGroup //
//////////////////////////
////////////////////////
//  Ray-tracing stuff //
////////////////////////
double TriangleListRayGroup::intersect( Ray3D ray , RayIntersectionInfo& iInfo , double mx )
{
	double dist = -1;
	int len = shapes.size();
	for (int i = 0; i < len; i++) {
		double val = shapes[i]->intersect(ray, iInfo, mx);
		if (val) {
			dist = val;
		}
	}
	return dist;
}

int TriangleListRayGroup::drawOpenGL( int materialIndex , GLSLProgram * glslProgram )
{
	throw RayException( "TriangleListRayGroup::drawOpenGL undefined" );
	return -1;
}

void TriangleListRayGroup::setUpOpenGL( int cplx , bool setBufferObjects )
{
	_vertexArrayID = _vertexBufferID = _elementBufferID = 0;
	throw RayException( "TriangleListRayGroup::setUpOpenGL undefined" );
}

/////////////////////
// Animation Stuff //
/////////////////////
Matrix4D ParametrizedEulerAnglesAndTranslation::getMatrix( void )
{
	throw RayException( "ParametrizedEulerAnglesAndTranslation::getMatrix undefined" );
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix( void )
{
	throw RayException( "ParametrizedClosestRotationAndTranslation::getMatrix undefined" );
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix( void )
{
	throw RayException( "ParametrizedRotationLogarithmAndTranslation::getMatrix undefined" );
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedQuaternionAndTranslation::getMatrix( void )
{
	throw RayException( "ParametrizedQuaternionAndTranslation::getMatrix undefined" );
	return Matrix4D::IdentityMatrix();
}
