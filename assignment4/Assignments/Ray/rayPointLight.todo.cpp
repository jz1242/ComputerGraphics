#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <Util/exceptions.h>
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getAmbient( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RayPointLight::getAmbient undefined" );
	return Point3D();
}
Point3D RayPointLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RayPointLight::getDiffuse undefined" );
	return Point3D();
}
Point3D RayPointLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RayPointLight::getSpecular undefined" );
	return Point3D();
}
bool RayPointLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape )
{
	Util::Throw( "RayPointLight::isInShadow undefined" );
	return false;
}
Point3D RayPointLight::transparency( RayIntersectionInfo& iInfo , RayShape* shape , Point3D cLimit )
{
	Util::Throw( "RayPointLight::transparency undefined" );
	return Point3D(1,1,1);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL( int index , GLSLProgram * glslProgram )
{
	GLfloat pos[] = { GLfloat(location[0]), GLfloat(location[1]), GLfloat(location[2]), 1 };
	GLfloat ambient[] = { GLfloat(aColor[0]), GLfloat(aColor[1]), GLfloat(aColor[2]) };
	GLfloat diffuse[] = { GLfloat(dColor[0]), GLfloat(dColor[1]), GLfloat(dColor[2]) };
	GLfloat specular[] = { GLfloat(sColor[0]), GLfloat(sColor[1]), GLfloat(sColor[2]) };
	glEnable(GL_LIGHT0 + index);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0 + index, GL_POSITION, pos);
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, specular);
	glLightf(GL_LIGHT0 + index, GL_CONSTANT_ATTENUATION, constAtten);
	glLightf(GL_LIGHT0 + index, GL_LINEAR_ATTENUATION, linearAtten);
	glLightf(GL_LIGHT0 + index, GL_QUADRATIC_ATTENUATION, quadAtten);
}