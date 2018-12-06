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
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getAmbient( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RaySpotLight::getAmbient undefined" );
	return Point3D();
}
Point3D RaySpotLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RaySpotLight::getDiffuse undefined" );
	return Point3D();
}
Point3D RaySpotLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RaySpotLight::getSpecular undefined" );
	return Point3D();
}
bool RaySpotLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape )
{
	Util::Throw( "RaySpotLight::isInShadow undefined" );
	return false;
}
Point3D RaySpotLight::transparency( RayIntersectionInfo& iInfo , RayShape* shape , Point3D cLimit )
{
	Util::Throw( "RaySpotLight::transparency undefined" );
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySpotLight::drawOpenGL( int index , GLSLProgram * glslProgram )
{
	GLfloat pos[] = { location[0], location[1], location[2], 1.0 };
	GLfloat dir[] = { direction[0],direction[1],direction[2] };
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
	glLightfv(GL_LIGHT0 + index, GL_SPOT_DIRECTION, dir);
	glLightf(GL_LIGHT0 + index, GL_SPOT_CUTOFF, cutOffAngle);
	glLightf(GL_LIGHT0 + index, GL_SPOT_EXPONENT, dropOffRate);
	glLightf(GL_LIGHT0 + index, GL_CONSTANT_ATTENUATION, constAtten);
	glLightf(GL_LIGHT0 + index, GL_LINEAR_ATTENUATION, linearAtten);
	glLightf(GL_LIGHT0 + index, GL_QUADRATIC_ATTENUATION, quadAtten);
}