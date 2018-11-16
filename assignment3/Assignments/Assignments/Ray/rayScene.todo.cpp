#include "rayScene.h"
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <math.h>


///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect( Point3D v , Point3D n )
{
	throw RayException( "RayScene::Reflect undefined" );
	return Point3D();
}

bool RayScene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	throw RayException( "RayScene::Refract undefined" );
	return 0;
}

Ray3D RayScene::GetRay( RayCamera* camera , int i , int j , int width , int height )
{
	throw RayException( "RayScene::GetRay undefined" );
	return Ray3D();
}

Point3D RayScene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	fprintf( stderr , "[WARNING] RayScene::getColor undefined\n" );
	return Point3D(1,1,1);
}
static void Point3DtoGLfloat(const Point3D &p, GLfloat *glf) {
	glf[0] = GLfloat(p[0]);
	glf[1] = GLfloat(p[1]);
	glf[2] = GLfloat(p[2]);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(GLSLProgram * glslProgram)
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLfloat a[4], e[4], d[4], s[4];
	Point3DtoGLfloat(ambient, a);
	Point3DtoGLfloat(emissive, e);
	Point3DtoGLfloat(diffuse, d);
	Point3DtoGLfloat(specular, s);
	a[3] = 1;
	e[3] = 1;
	d[3] = 1;
	s[3] = 1;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, e);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, specularFallOff);
}

void RayTexture::setUpOpenGL( void )
{
	glGenTextures(1, &openGLHandle);
	glBindTexture(GL_TEXTURE_2D, openGLHandle);
	GLvoid *tex_img = (GLvoid *) &(*img)(0, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width(), img->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_img);
	glBindTexture(GL_TEXTURE_2D, 0);
}
