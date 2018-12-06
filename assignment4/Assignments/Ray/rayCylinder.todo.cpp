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
#include "rayCylinder.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCylinder::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	//Util::Throw("Undefined");
	return -1;
}

BoundingBox3D RayCylinder::setBoundingBox(void)
{
	Point3D p(radius, height / 2, radius);
	bBox = BoundingBox3D(center + p, center - p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RayCylinder::setUpOpenGL(int cplx, bool setBufferObjects)
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	//Util::Throw("Undefined");
}

int RayCylinder::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	if (materialIndex != material->index) material->drawOpenGL(glslProgram);

	GLUquadric* q = gluNewQuadric();
	gluQuadricTexture(q, true);

	glTranslatef(center.p[0], center.p[1] - height / 2, center.p[2]);
	glRotatef(90, -1, 0, 0);
	gluCylinder(q, radius, radius, height, openGLComplexity, openGLComplexity);

	glPushMatrix();
	glTranslatef(0, 0, height);
	gluDisk(q, 0, radius, openGLComplexity, openGLComplexity);
	glPopMatrix();

	glRotatef(180, 1, 0, 0); // Normals pointing out
	gluDisk(q, 0, radius, openGLComplexity, openGLComplexity);

	return material->index;
}