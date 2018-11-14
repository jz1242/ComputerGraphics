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
	GLfloat light_dir[4] = { GLfloat(-direction[0]), GLfloat(-direction[1]), GLfloat(-direction[2]), 0.0 };
	glLightfv(GL_LIGHT0 + index, GL_POSITION, light_dir);

	GLfloat acol[3] = { GLfloat(aColor[0]), GLfloat(aColor[1]), GLfloat(aColor[2]) };
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, acol);

	GLfloat spec_col[3] = { GLfloat(sColor[0]), GLfloat(sColor[1]), GLfloat(sColor[2]) };
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, spec_col);

	GLfloat diff_col[3] = { GLfloat(dColor[0]), GLfloat(dColor[1]), GLfloat(dColor[2]) };
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, diff_col);

	//printf("Let there be directional light\n");
	glEnable(GL_LIGHT0 + index);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}