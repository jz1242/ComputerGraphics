#ifndef SHAPE_INCLUDED
#define SHAPE_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdexcept>
#include <string>
#include <Util/geometry.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#ifdef NO_OPEN_GL
struct GLSLProgram
{
	GLSLProgram( const std::string & , const std::string & ){ }
	void setup( void ){ }
};
#else // !NO_OPEN_GL
#include "GLSLProgram.h"
#endif // NO_OPEN_GL

/** This struct represents an exception thrown by one of the ray classes. */
struct RayException : public std::runtime_error
{
	RayException( const std::string & msg ) : std::runtime_error( std::string( "Ray: " ) + msg ) {}
};

/** This is the abstract class that all ray-traceable objects must implement. */
class RayShape
{
public:
	/** A simple structure for storing a triplet of triangle indices.*/
	struct TriangleIndex
	{
		GLuint indices[3];
		TriangleIndex( void ){ indices[0] = indices[1] = indices[2] = 0; }
		TriangleIndex( unsigned int i0 , unsigned int i1 , unsigned int i2 ){ indices[0] = i0 , indices[1] = i1 , indices[2] = i2; }

		GLuint& operator[] ( int idx ){ return indices[idx]; }
		GLuint  operator[] ( int idx ) const { return indices[idx]; }
	};

	/** The bounding box of the shape */
	BoundingBox3D bBox;

	/** This method writes the RayShape out to a .ray file, indenting the output by the specified amount.
	  * If no file is specified, the information gets written to stdout.
	  */
	virtual void write( int indent , FILE* fp=stdout )=0;
	

	///////////////////////
	// Ray-Tracing stuff //
	///////////////////////
	/** This method computes the intersection of the shape with the ray.
	  * If the ray hits the shape and either mx is less than zero or the intersection
	  * occurs at a distance less than t along the ray the intersection information in iInfo is
	  * set and a value greater than 0 is returned. Otherwise, -1 is returned. (This is useful for
	  * shadow testing when you may want to ignore hits beyond a certain distance).
	  */
	virtual double intersect( Ray3D ray , struct RayIntersectionInfo& iInfo , double mx=-1 )=0;
	
	/** This method sets and returns a bounding box containing the shape.*/
	virtual BoundingBox3D setBoundingBox( void )=0;


	//////////////////
	// OpenGL stuff //
	//////////////////
	/** The complexity for the rasterization of the shape */
	int openGLComplexity;

	/** This pure virtual method sets up all the stuff that's necessary prior to OpenGL rendering.
	* The value cplx represents the desired resolution of tessalation for objects such as spheres, cylinders, etc. */
	virtual void setUpOpenGL( int cplx , bool setBufferObjects )=0;

	/** This pure virtual method calls the necessary OpenGL commands to render the primitive. */
	virtual int drawOpenGL( int materialIndex , GLSLProgram * glslProgram )=0;

	/** This virtual method adds a triangle to the list of triangles (if the object is of type RayTriangle). */
	virtual void addTrianglesOpenGL( std::vector< TriangleIndex >& triangles ) {}
};
#endif // SHAPE_INCLUDED
