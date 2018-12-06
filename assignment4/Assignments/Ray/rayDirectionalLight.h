#ifndef DIRECTIONAL_LIGHT_INCLUDED
#define DIRECTIONAL_LIGHT_INCLUDED
#include "rayLight.h"

/** This class represents a directional light source. The light has no position
  * in space and hence has no attenuation properties. */
class RayDirectionalLight : public RayLight
{
public:
	/** The direction the outgoing light rays */
	Point3D direction;

	/** This method reads in the directional light from a .ray file. */
	void read( FILE* fp );

	void write( FILE* fp=stdout );
		
	///////////////////////
	// Ray-Tracing stuff //
	///////////////////////
	Point3D getAmbient( Point3D cameraPosition , struct RayIntersectionInfo& iInfo );
	Point3D getDiffuse( Point3D cameraPosition , struct RayIntersectionInfo& iInfo );
	Point3D getSpecular( Point3D cameraPosition , struct RayIntersectionInfo& iInfo );	
	bool isInShadow( struct RayIntersectionInfo& iInfo , class RayShape* shape );
	Point3D transparency( struct RayIntersectionInfo& iInfo , class RayShape* shape , Point3D cLimit );


	//////////////////
	// OpenGL stuff //
	//////////////////
	void drawOpenGL( int index , GLSLProgram * glslProgram );
};
	
#endif // DIRECTIONAL_LIGHT_INCLUDED
	
	
