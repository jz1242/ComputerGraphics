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
	throw RayException( "RayDirectionalLight::getAmbient undefined" );
	return Point3D();
}
Point3D RayDirectionalLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	throw RayException( "RayDirectionalLight::getDiffuse undefined" );
	return Point3D();
}
Point3D RayDirectionalLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	throw RayException( "RayDirectionalLight::getSpecular undefined" );
	return Point3D();
}
bool RayDirectionalLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape , int& isectCount )
{
	throw RayException( "RayDirectionalLight::isInShadow undefined" );
	return false;
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