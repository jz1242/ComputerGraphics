#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getAmbient( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	return iInfo.material->ambient * aColor;
}
Point3D RayPointLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	return Point3D(0, 0, 0);
}
Point3D RayPointLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	return Point3D(0, 0, 0);
}
bool RayPointLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape , int& isectCount )
{
	throw RayException( "RayPointLight::isInShadow undefined" );
	return false;
}
Point3D RayPointLight::transparency( RayIntersectionInfo& iInfo , RayShape* shape , Point3D cLimit )
{
	throw RayException( "RayPointLight::transparency undefined" );
	return Point3D(1,1,1);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL( int index , GLSLProgram * glslProgram )
{
	throw RayException( "RayPointLight::drawOpenGL undefined" );
}