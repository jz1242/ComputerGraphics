#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "raySphere.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	//throw RayException("RaySphere::intersect undefined");
	return -1;
}

BoundingBox3D RaySphere::setBoundingBox(void)
{
	Point3D p(radius, radius, radius);
	bBox = BoundingBox3D(center + p, center - p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
void RaySphere::setUpOpenGL(int cplx, bool setBufferObjects)
{
	openGLComplexity = cplx;
	_vertexArrayID = _elementBufferID = _vertexBufferID = 0;
	//throw RayException("RaySphere::setUpOpenGL undefined");
}

int RaySphere::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	if (materialIndex != material->index) {
		material->drawOpenGL(glslProgram);
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(center.p[0], center.p[1], center.p[2]);
	GLfloat a;
	GLfloat b;
	GLfloat c; 

	double res = PI / 100;

	for (double i = 0; i < PI; i += res)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (double j = 0; j < 2*PI; j += res)
		{
			a = radius * cos(j)*sin(i);
			b = radius * sin(j)*sin(i);
			c = radius * cos(i);
			glVertex3f(a, b, c);
			glNormal3f(a, b, c);
			a = radius * cos(j)*sin(i + res);
			b = radius * sin(j)*sin(i + res);
			c = radius * cos(i + res);
			glVertex3f(a, b, c);
			glNormal3f(a, b, c);
		}
		glEnd();
	}
	glFlush();
	glPopMatrix();
	glPopAttrib();
	return material->index;
}