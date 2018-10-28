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
	RayIntersectionInfo tmp; // so we dont change iInfo
	Ray3D transformRayWorld = getInverseMatrix() * ray;
	transformRayWorld.direction = transformRayWorld.direction.unit();
	double val = mx;
	for (int i = 0; i < shapes.size(); i++) {
		double isIntersected = shapes[i]->intersect(transformRayWorld, tmp, val);
		if (isIntersected > 0) {
			Point3D worldDist = (tmp.iCoordinate - (getMatrix() * transformRayWorld).position);
			val = worldDist.length();
		}
	}
	if (mx > 0 && val >= mx) {
		return -1.0;
	}

	iInfo.material = tmp.material;
	iInfo.normal = getNormalMatrix().multDirection(tmp.normal).unit();
	iInfo.iCoordinate = getMatrix().multPosition(tmp.iCoordinate);
	return val;
}

BoundingBox3D RayGroup::setBoundingBox( void )
{
	return bBox;
}

bool StaticRayGroup::set( void )
{
	inverseTransform = localTransform.inverse();
	normalTransform = localTransform.transpose().inverse();
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
	double dist = mx;
	for (int i = 0; i < shapes.size(); i++) {
		double intersect_dist = shapes[i]->intersect(ray, iInfo, dist);
		if (intersect_dist > 0) {
			dist = intersect_dist;
		}
	}
	if (mx > 0 && dist >= mx) {
		return -1.0;
	}
	iInfo.material = material;
	iInfo.iCoordinate = ray.position + ray.direction * dist;
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
