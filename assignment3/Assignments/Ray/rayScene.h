#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED
#include <Util/geometry.h>
#include <Image/image.h>
#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else // !__APPLE__
#include <GL/glut.h>
#endif // __APPLE__
#include <vector>
#include "rayShape.h"
#include "rayLight.h"
#include "rayGroup.h"
#include "rayKey.h"
#include "rayCamera.h"

#define STRING_BUFFER_SIZE 200

/** If a ray intersects a shape, the shape information at the point of intersection is stored in this structure. */
struct RayIntersectionInfo
{
	/** The material of the intersected surface */
	class RayMaterial* material;
	/** The position, in world coordinates, of the intersection */
	Point3D iCoordinate;
	/** The normal of the shape at the point of intersection */
	Point3D normal;
	/** The texture coordinates of the the shape at the point of intersection */
	Point2D texCoordinate;
};

/** This class stores surface material properties. */
class RayMaterial
{
public:
	/** The index of the material within the scene */
	int index;
	/** The amount of ambient light the surface transmits */
	Point3D ambient;
	/** The amount of diffuse light the surface transmits */
	Point3D diffuse;
	/** The amount of specular light the surface transmits */
	Point3D specular;
	/** The amount of light the surface emits */
	Point3D emissive;
	/** The amount of transparent light the surface transmits */
	Point3D transparent;
	/** The coefficient for the specular fall-off of the material */
	double specularFallOff;
	/** The refraction index of the material */
	double refind;
	/** A pointer to the texture associated with a surface */
	class RayTexture *tex;
	/** A string for storing additional material parameters specified by the user */
	char foo[STRING_BUFFER_SIZE];

	RayMaterial( void );

	/** This method reads in the material information from a .ray file.*/
	void read( FILE * fp , int & textureIndex );
	/** This method writes out the material information to a .ray file. If no file is specified,
	  * the material is written out to stdout. */
	void write( FILE * fp=stdout );
	
	/** This method calls the OpenGL commands for setting up the material. */
	void drawOpenGL( GLSLProgram * glslProgram );
};

/** This class stores information about a texture used for texture mapping shapes */
class RayTexture
{
public:
	/** The index of the texture within the scene */
	int index;
	/** The name of the texture file */
	char filename[STRING_BUFFER_SIZE];
	/** The image used as a texture */
	Image32 *img;

	RayTexture( void );
	~RayTexture( void );
	/** This method reads in the texture file name from the .ray file and reads the texture from the specified file.*/
	void read( FILE * fp );
	/** This method writes out the texture file name to a .ray file. If no file is specified,
	  * the file name is written out to stdout. */
	void write( FILE * fp=stdout );

	/** The texture handle for OpenGL rendering */
	GLuint openGLHandle;

	/** This method sets up the OpenGL texture */
	void setUpOpenGL( void );
};

/** This class stores information about a vertex shader */
class RayShader
{
public:
	/** The name of the vertex shader file */
	char vertexShaderFilename[STRING_BUFFER_SIZE];
	/** The name of the fragment shader file */
	char fragmentShaderFilename[STRING_BUFFER_SIZE];
	/** The shader */
	GLSLProgram * glslProgram;

	RayShader( void );
	~RayShader( void );

	/** This method reads in the vertex and fragment shader file names from the .ray file and reads the shaders from the specified files.*/
	void read( FILE * fp );
	/** This method writes out the vertex and fragment and shader file names to a .ray file. If no file is specified, the file name is written out to stdout. */
	void write( FILE * fp=stdout );
};

/** This class stores information about the scene-graph read out from a .ray file. */
class RayFile
{
public:
	/** The index of the .ray file within the scene */
	int index;
	/** The name of the .ray file */
	char filename[STRING_BUFFER_SIZE];
	/** The scene graph associated with a .ray file */
	class RayScene* scene;

	~RayFile( void );

	/** This method reads in ray-file name from the .ray file and reads the scene-graph from the specified file.*/
	void read( FILE * fp , int transformType );
	/** This method writes out ray-file name to a .ray file. If no file is specified,
	  * the ray-file name is written out to stdout. */
	void write( FILE * fp=stdout );
};

/** This class stores all of the information read out from a .ray file.*/
class RayScene
{
	std::vector< RayMaterial * > _materials;
	std::vector< RayTexture * > _textures;
	std::vector< RayFile * > _rayFiles;
	std::vector< std::pair< RayKeyFile , RayKeyData > * > _keyFileData;

	void ParseError( const char * );
	void ParseLineError( int , const char * );
	void ParseGroup       ( FILE * fp , int & cmndCtr , class RayGroup * current );
	void ParseJoint       ( FILE * fp , int & cmndCtr , class RayGroup * current );
	void ParseCluster     ( FILE * fp , int & cmndCtr , class RayGroup * current , std::vector< class RayVertex > * vList=NULL );
	void ParseTriangleList( FILE * fp , int & cmndCtr , class RayGroup * current );
	bool ParseShape( FILE * fp , const char * keyword , int cmndCtr , class RayGroup * current , std::vector< class RayVertex > * vList=NULL );
public:
	/** The color of the background */
	Point3D background;
	/** The camera */
	RayCamera* camera;
	/** The shader */
	RayShader* shader;
	/** The list of lights in the scene */
	std::vector< RayLight * > lights;
	/** The root of the scene-graph */
	class StaticRayGroup* group;

	RayScene( void );
	~RayScene( void );

	/** This method returns the material with the specified index.*/
	RayMaterial* getMaterial( int index );

	/** This method reads in the scene-graph from a .ray file. */
	void read( char * fileName , int transformType=RayKeyData::REPRESENTATION_MATRIX );
	/** This method reads in the scene-graph from a .ray file. */
	void read( FILE * fp , int transformType );
	/** This method writes out the scene-graph to a .ray file. */
	void write( char * fileName );
	/** This method writes out the scene-graph to a .ray file. If no file is specified the scene-graph is written out stdout. */
	void write( FILE * fp=stdout );

	///////////////////////
	// Ray-Tracing stuff //
	///////////////////////
	Image32 rayTrace( int width , int height , int rLimit , double cLimit );

	/** This function will reflect the vector v about the normal n */
	static Point3D Reflect( Point3D v , Point3D n );

	/** This function will refract the vector about the normal using the provided indices of refraction
	  * The refracted vector is written into refract and a value of 1 is returned if the refraction
	  * succeeded (i.e. the necessary arcsin could be computed)
	  */
	static bool Refract( Point3D v , Point3D n , double ir , Point3D& refract );

	/** This function returns that leaves the camera and goes through pixel (i,j) of the view plane */
	static Ray3D GetRay( RayCamera * camera , int i , int j , int width , int height );

	/** This is the function responsible for the recursive ray-tracing returning the color obtained
	  * by shooting a ray into the scene and recursing until either the recursion depth has been reached
	  * or the contribution from subsequent bounces is guaranteed to be less than the cut-off.\n
	  */
	Point3D getColor( Ray3D ray , int rDepth , Point3D cLimit );

	//////////////////
	// OpenGL stuff //
	//////////////////

	/** This method calls the OpenGL commands for drawing the scene. */
	void drawOpenGL( void );

	/** This method class the OpenGL commands to set up everything that needs to be set up prior to rendering */
	void setUpOpenGL( int cplx );

	/** This method updates the current time, changing the parameter values as needed */
	void setCurrentTime( double t , int curveFit=INTERPOLATION_LINEAR );
};

#endif // SCENE_INCLUDED
  

