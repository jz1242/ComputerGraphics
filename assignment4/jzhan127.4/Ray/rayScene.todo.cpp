#include "rayScene.h"
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <math.h>
#include <Util/exceptions.h>


///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect( Point3D v , Point3D n )
{
	Util::Throw( "RayScene::Reflect undefined" );
	return Point3D();
}

bool RayScene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	Util::Throw( "RayScene::Refract undefined" );
	return 0;
}

Ray3D RayScene::GetRay( RayCamera* camera , int i , int j , int width , int height )
{
	Util::Throw( "RayScene::GetRay undefined" );
	return Ray3D();
}

Point3D RayScene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	Util::Warn( "RayScene::getColor undefined" );
	return Point3D(1,1,1);
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL( GLSLProgram * glslProgram )
{
	GLfloat amb[] = { GLfloat(ambient[0]), GLfloat(ambient[1]), GLfloat(ambient[2]), 1.0 };
	GLfloat dif[] = { GLfloat(diffuse[0]), GLfloat(diffuse[1]), GLfloat(diffuse[2]), 1.0 };
	GLfloat spec[] = { GLfloat(specular[0]), GLfloat(specular[1]), GLfloat(specular[2]), 1.0 };
	GLfloat em[] = { GLfloat(emissive[0]), GLfloat(emissive[1]), GLfloat(emissive[2]), 1.0 };
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, specularFallOff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, em);
}
void RayTexture::setUpOpenGL( void )
{
	glGenTextures(1, &openGLHandle);
	glBindTexture(GL_TEXTURE_2D, openGLHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width(), img->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *) &(*img)(0, 0));
}
