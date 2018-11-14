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
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLUquadricObj *obj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(center.p[0], center.p[1], center.p[2]);
	material->drawOpenGL(glslProgram);
	//material->drawOpenGL();
	//gluSphere(obj, radius, openGLComplexity, openGLComplexity);
	//gluDeleteQuadric(obj);
	//glFlush();

	GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles
	//GLfloat radius = radius;
	int gradation = 50;

	for (alpha = 0.0; alpha < 3.1415; alpha += PI / gradation)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (beta = 0.0; beta < 2.01*3.1415; beta += PI / gradation)
		{
			x = radius * cos(beta)*sin(alpha);
			y = radius * sin(beta)*sin(alpha);
			z = radius * cos(alpha);
			glVertex3f(x, y, z);
			//glNormal3f(x, y, z);

			x = radius * cos(beta)*sin(alpha + PI / gradation);
			y = radius * sin(beta)*sin(alpha + PI / gradation);
			z = radius * cos(alpha + PI / gradation);
			glVertex3f(x, y, z);
			glNormal3f(x, y, z);
		}
		glEnd();
	}
	glFlush();
	glPopMatrix();
	glPopAttrib();
	return -1;
}