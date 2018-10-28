#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayDirectionalLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getAmbient( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	return aColor * iInfo.material->ambient;
}
Point3D RayDirectionalLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	double angle = Point3D::Dot(iInfo.normal, -direction);
	if (angle > 1) {
		angle = 1;
	}
	if (angle < 0) {
		angle = 0;
	}
	return iInfo.material->diffuse* angle*dColor;
}
Point3D RayDirectionalLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Point3D normal = iInfo.normal;
	Point3D light = -direction.unit(); //light direction vector opposite direction
	Point3D viewer = (cameraPosition - iInfo.iCoordinate).unit(); //direction vector towards viewer
	double theta = Point3D::Dot(normal, light);
	if (theta > 1) {
		theta = 1;
	}
	if (theta < 0) {
		theta = 0;
	}
	Point3D ref = ((normal* theta)* 2  - light).unit();
	double alpha = Point3D::Dot(viewer, ref);
	if (alpha > 1) {
		alpha = 1;
	}
	if (alpha < 0) {
		alpha = 0;
	}
	double angle = pow(alpha, iInfo.material->specularFallOff); //n value
	return sColor*angle *iInfo.material->specular;
}
bool RayDirectionalLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape , int& isectCount )
{
	Ray3D shadow_ray = Ray3D(iInfo.iCoordinate + Point3D(RAYEPS, RAYEPS, RAYEPS), -direction.unit()); //shift by epsilon
	double intersect = shape->intersect(shadow_ray, *new RayIntersectionInfo());
	return intersect > 0 ? true : false;
}
Point3D RayDirectionalLight::transparency( RayIntersectionInfo& iInfo , RayShape* shape , Point3D cLimit )
{
	throw RayException( "RayDirectionalLight::transparency undefined" );
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL( int index , GLSLProgram * glslProgram )
{
	throw RayException( "RayDirectionalLight::drawOpenGL undefined" );
}