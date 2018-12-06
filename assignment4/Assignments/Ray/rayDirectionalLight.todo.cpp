#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <Util/exceptions.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getAmbient( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RayDirectionalLight::getAmbient undefined" );
	return Point3D();
}
Point3D RayDirectionalLight::getDiffuse( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RayDirectionalLight::getDiffuse undefined" );
	return Point3D();
}
Point3D RayDirectionalLight::getSpecular( Point3D cameraPosition , RayIntersectionInfo& iInfo )
{
	Util::Throw( "RayDirectionalLight::getSpecular undefined" );
	return Point3D();
}
bool RayDirectionalLight::isInShadow( RayIntersectionInfo& iInfo , RayShape* shape )
{
	Util::Throw( "RayDirectionalLight::isInShadow undefined" );
	return false;
}
Point3D RayDirectionalLight::transparency( RayIntersectionInfo& iInfo , RayShape* shape , Point3D cLimit )
{
	Util::Throw( "RayDirectionalLight::transparency undefined" );
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL( int index , GLSLProgram * glslProgram )
{
	glEnable(GL_LIGHT0 + index);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat dir_l[] = { GLfloat(-direction[0]), GLfloat(-direction[1]), GLfloat(-direction[2]), 0.0 };
	GLfloat ambient[] = { GLfloat(aColor[0]), GLfloat(aColor[1]), GLfloat(aColor[2]) };
	GLfloat diffuse[] = { GLfloat(dColor[0]), GLfloat(dColor[1]), GLfloat(dColor[2]) };
	GLfloat specular[] = { GLfloat(sColor[0]), GLfloat(sColor[1]), GLfloat(sColor[2]) };
	glLightfv(GL_LIGHT0 + index, GL_POSITION, dir_l);
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, specular);
}