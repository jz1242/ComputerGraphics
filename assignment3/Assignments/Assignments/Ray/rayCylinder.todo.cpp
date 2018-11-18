#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include "rayScene.h"
#include "rayCylinder.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCylinder::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx)
{
	throw RayException("RayCylinder::intersect undefined");
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
}

int RayCylinder::drawOpenGL(int materialIndex, GLSLProgram * glslProgram)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if (materialIndex != material->index) {
		material->drawOpenGL(glslProgram);
	}
	int sides = openGLComplexity, slices = openGLComplexity;

	const float theta = 2. * PI / sides;
	const float c = cos(theta);
	const float s = sin(theta);
	const float x = center[0];
	const float y = center[1] - height * 0.5;
	const float z = center[2];

	// Barrel of the cylinder 
	float x2 = radius, z2 = 0;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= sides; i++) {
		// texture coord
		const float tx = float(i) / sides;
		const float nf = 1. / sqrt(x2*x2 + z2 * z2);
		const float xn = x2 * nf;
		const float zn = z2 * nf;
		glNormal3f(xn, 0, zn);
		glTexCoord2f(tx, 0);
		glVertex3f(x + x2, y, z + z2);
		glNormal3f(xn, 0, zn);
		glTexCoord2f(tx, 1);
		glVertex3f(x + x2, y + height, z + z2);
		// next position
		const float x3 = x2;
		x2 = c * x2 - s * z2;
		z2 = s * x3 + c * z2;
	}
	glEnd();
	glFlush();

	glBegin(GL_TRIANGLE_FAN);
	float cy = center[1] + 0.5 * height;
	glVertex3f(x, cy, z);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for (int k = sides; k >= 0; --k) {
		glVertex3f(x + radius * cos(theta * k), cy, z + radius * sin(theta * k));
		glNormal3f(0.0f, 1.0f, 0.0f);
	}
	glEnd();
	glFlush();

	glBegin(GL_TRIANGLE_FAN);
	cy = center[1] - 0.5 * height;
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(x, cy, z);
	for (int k = 0; k <= sides; ++k) {
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(x + radius * cos(theta * k), cy, z + radius * sin(theta * k));
	}
	glEnd();

	glFlush();
	glPopAttrib();
	return material->index;
}