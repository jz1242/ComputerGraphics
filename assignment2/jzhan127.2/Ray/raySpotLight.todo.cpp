#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getAmbient( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	return aColor * iInfo.material->ambient;
}
Point3D RaySpotLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	throw RayException( "RaySpotLight::getDiffuse undefined" );
	return Point3D();
}
Point3D RaySpotLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	throw RayException( "RaySpotLight::getSpecular undefined" );
	return Point3D();
}
bool RaySpotLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape , int& isectCount )
{
	throw RayException( "RaySpotLight::isInShadow undefined" );
	return false;
}
Point3D RaySpotLight::transparency( RayIntersectionInfo& iInfo , RayShape* shape , Point3D cLimit )
{
	throw RayException( "RaySpotLight::transparency undefined" );
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySpotLight::drawOpenGL( int index , GLSLProgram * glslProgram )
{
	throw RayException( "RaySpotLight::drawOpenGL undefined" );
}