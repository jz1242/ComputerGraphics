#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Image/bmp.h>
#include "rayScene.h"
#include "rayPointLight.h"
#include "rayDirectionalLight.h"
#include "raySpotLight.h"
#include "rayFileInstance.h"
#include "raySphere.h"
#include "rayBox.h"
#include "rayCone.h"
#include "rayCylinder.h"
#include "rayTriangle.h"
#include "rayGroup.h"

const static int BUF_SIZE=500;

/////////////////
// RayMaterial //
/////////////////
RayMaterial::RayMaterial( void ) : index(-1) , specularFallOff(0) , refind(1) , tex(NULL) { foo[0]=0; }
void RayMaterial::read( FILE* fp , int& tIdx )
{
	double *e = &emissive[0] , *a = &ambient[0] , *d = &diffuse[0] , *s = &specular[0] , *f = &specularFallOff , *t = &transparent[0] , *r = &refind;
	if( fscanf( fp , " %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %d !%s!" , e+0 , e+1 , e+2 , a+0 , a+1 , a+2 , d+0 , d+1 , d+2 , s+0 , s+1 , s+2 , f , t+0 , t+1 , t+2 , r , &tIdx , foo )!=19 )
	{
		throw RayException( std::string( "Failed to parse material\n" ) );
	}
	else if( foo[strlen(foo)-1]=='!' ) foo[strlen(foo)-1] = '\0';
}
void RayMaterial::write( FILE* fp )
{
	int tIdx;
	
	if( !tex ) tIdx = -1;
	else       tIdx = tex->index;
	fprintf( fp , "#material\n");
	fprintf( fp , "   %lg %lg %lg\n" , emissive[0] , emissive[1] , emissive[2] );
	fprintf( fp , "   %lg %lg %lg\n" , ambient[0] , ambient[1] , ambient[2] );
	fprintf( fp , "   %lg %lg %lg\n" , diffuse[0] , diffuse[1] , diffuse[2] );
	fprintf( fp , "   %lg %lg %lg %lg\n" , specular[0] , specular[2] , specular[2] , specularFallOff );
	fprintf( fp , "   %lg %lg %lg\n" , transparent[0] , transparent[1] , transparent[2] );
	fprintf( fp , "   %lg\n" , refind );
	fprintf( fp , "   %d\n" , tIdx );
	fprintf( fp , "   !%s!\n" , foo );
}

////////////////
// RayTexture //
////////////////
RayTexture::RayTexture( void ) : index(0) , img(NULL) { filename[0] = 0; }
RayTexture::~RayTexture( void ){ if( img ) delete img; }

void RayTexture::read( FILE* f )
{
	if( fscanf( f , " %s" , filename )!= 1 ) throw RayException( std::string( "Failed to parse texture\n" ) ); 
	img = new Image32();
	if( !img ) throw RayException( std::string( "Failed to allocate image\n" ) );
	img->read( filename );
}
void RayTexture::write( FILE* fp )
{
	fprintf( fp , "#texture\n" );
	fprintf( fp , "   %s\n" , filename );
}

///////////////
// RayShader //
///////////////
RayShader::RayShader( void ) : glslProgram(NULL) { vertexShaderFilename[0] = fragmentShaderFilename[0] = 0; }
RayShader::~RayShader( void ){ if( glslProgram ) delete glslProgram; }

void RayShader::read( FILE* f )
{
	if( fscanf( f , " %s %s" , vertexShaderFilename , fragmentShaderFilename )!= 2 ) throw RayException( std::string( "Failed to parse shader\n" ) );
	glslProgram = new GLSLProgram( vertexShaderFilename , fragmentShaderFilename );
	if( !glslProgram ) throw RayException( std::string( "Failed to construct shader\n" ) );
}
void RayShader::write( FILE* fp )
{
	fprintf( fp , "#shader\n" );
	fprintf( fp , "   %s %s\n" , vertexShaderFilename , fragmentShaderFilename );
}

/////////////
// RayFile //
/////////////
RayFile::~RayFile( void ){ if( scene ) delete scene; }
void RayFile::read( FILE* fp , int transformType )
{
	scene = new RayScene();
	if( !scene ) throw RayException( std::string( "Failed to allocate scene\n" ) );
	if( fscanf( fp , " %s" , filename )!=1 )
	{
		delete scene;
		scene = NULL;
		throw RayException( std::string( "Failed to parse ray_file\n" ) );
	}
	scene->read( filename , transformType );
}
void RayFile::write( FILE* fp )
{
	fprintf( fp , "#ray_file\n" );
	fprintf( fp , "   %s\n" , filename );
}

//////////////
// RayScene //
//////////////
RayScene::RayScene( void ) : background(1.,1.,1.) , group(NULL) , camera(NULL) {}
RayMaterial* RayScene::getMaterial( int index )
{
	if( index<0 || index>=_materials.size() ) return NULL;
	return _materials[index];
}
RayScene::~RayScene( void )
{
	for( int i=0 ; i<lights.size() ; i++ ) delete lights[i];

	for( int i=0 ; i<_materials.size() ; i++ ) delete _materials[i];
	for( int i=0 ; i<_textures.size() ; i++ ) delete _textures[i];
	for( int i=0 ; i<_rayFiles.size() ; i++ ) delete _rayFiles[i];
	for( int i=0 ; i<_keyFileData.size() ; i++ ) delete _keyFileData[i];
	if( group ) delete group;
	if( camera ) delete camera;
}
void RayScene::write( char* fileName )
{
	if( !fileName ) write( stdout );
	else
	{
		FILE* fp = fopen( fileName , "w" );
		if( !fp ) throw( RayException( std::string( "Failed to open file for reading: " ) + fileName ) );
		write( fp );
		fclose( fp );
	}
}
void RayScene::write( FILE* fp )
{
	if( camera ) camera->write(fp);
	if( background[0] || background[1] || background[2] ) fprintf( fp , "#background\n   %lg %lg %lg\n" , background[0] , background[1] , background[2] );
	for( int i=0 ; i<lights.size() ; i++ ) lights[i]->write(fp);
	for( int i=0 ; i<_textures.size() ; i++ ) _textures[i]->write(fp);
	for( int i=0 ; i<_materials.size() ; i++ ) _materials[i]->write(fp);
	for( int i=0 ; i<_rayFiles.size() ; i++ ) _rayFiles[i]->write(fp);
	for( int i=0 ; i<group->shapes.size() ; i++ ) group->shapes[i]->write( 0 , fp );
}

void RayScene::read( char* fileName , int transformType )
{
	FILE* fp = fopen( fileName , "r" );
	if( !fp ) throw RayException( std::string( "Failed to open file for reading: " ) + fileName );
	read( fp , transformType );
	fclose( fp );
}
void RayScene::read( FILE* fp , int transformType )
{
	char keyword[BUF_SIZE];
	int cmndCtr = 0;
	bool inShapes = false , backFlag = false , ambFlag = false , shaderFlag = false;
	RayGroup* root;
	
	group = new StaticRayGroup();
	group->set( Matrix4D::IdentityMatrix() );
	root = group;
		
	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		
		// We try to read out the camera information. We make sure that we have
		// not begun describing the scene graph yet, and we ignore the camera if 
		// it is the second one specified.
		if( !strcmp( keyword , "camera" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			if( camera ) fprintf( stderr , "Parsing Line %d: Ignoring extra camera\n" , cmndCtr );
			else
			{
				camera = new RayCamera();
				if( !camera ) ParseLineError( cmndCtr , "failed to allocate camera\n" );
				camera->read( fp );
			}
		}
		
		// Next we try to read out the environment information. Again, we make
		// sure that we have not begun describing the scene graph yet and we 
		// ignore all but the first background color and ambient color 
		// specifications.
		else if( !strcmp( keyword , "background" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			if( backFlag==1 ) fprintf( stderr , "Parsing Line %d: Ignoring extra background\n" , cmndCtr );
			else
			{
				backFlag = true;
				if( fscanf( fp , " %lg %lg %lg" , &(background[0]) , &(background[1]) , &(background[2]) )!=3 ) ParseLineError( cmndCtr , "failed to parse background settings\n" );
			}
		}
		
		//Next we try read out the next textures. We make sure that we have
		//not begun describing the scene graph. We make sure that we are not
		//adding more textures then we promised and we make sure that all the
		//file I/O works as it should.
		else if( !strcmp( keyword , "texture" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			RayTexture* rayTexture = new RayTexture();
			if( !rayTexture ) ParseLineError( cmndCtr , "could not allocate RayTexture\n" );
			rayTexture->read( fp );
			rayTexture->index = (int)_textures.size();
			_textures.push_back( rayTexture );
		}

		//Next we try read out the next material. We make sure that we have
		//not begun describing the scene graph. We make sure that we are not
		//adding more materials then we promised.
		else if( !strcmp( keyword , "material" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			RayMaterial* rayMaterial = new RayMaterial();
			if( !rayMaterial ) ParseLineError( cmndCtr , "could not allocate RayMaterial\n" );
			int tIdx;
			rayMaterial->read( fp , tIdx );
			rayMaterial->tex = (RayTexture*)(size_t)tIdx;
			rayMaterial->index = (int)_materials.size();
			_materials.push_back( rayMaterial );
		}
		
		//Next we try to read out the next rayFile
		else if( !strcmp( keyword , "ray_file" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			RayFile* rayFile = new RayFile();
			if( !rayFile ) ParseLineError( cmndCtr , "could not allocate RayFile\n" );
			rayFile->read( fp , transformType );
			rayFile->index = (int)_rayFiles.size();
			_rayFiles.push_back( rayFile );
		}
		
		//Next we try read out the next light. We make sure that we have
		//not begun describing the scene graph. We make sure that we are not
		//adding more materials then we promised.
		else if( !strcmp( keyword,"light_point" ) || !strcmp( keyword , "light_dir" ) || !strcmp( keyword , "light_spot" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			RayLight* light = NULL;
			if     ( !strcmp( keyword , "light_point" ) ) light = new RayPointLight();
			else if( !strcmp( keyword , "light_dir"   ) ) light = new RayDirectionalLight();
			else if( !strcmp( keyword , "light_spot"  ) ) light = new RaySpotLight();
			if( !light ) ParseLineError( cmndCtr , "failed to allocate memory for light\n" );
			else light->read( fp );
			lights.push_back( light );
		}

		//Next we try to read out the .key file. We make sure that we have not
		//begun describing the scene graph.
		else if( !strcmp( keyword , "key_file" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			std::pair< RayKeyFile , RayKeyData >* keyFileData = new std::pair< RayKeyFile , RayKeyData >();
			if( !keyFileData ) ParseLineError( cmndCtr , "could not allocate std::pair< RayKeyFile , RayKeyData >\n" );
			char keyFileName[BUF_SIZE];
			if( fscanf( fp , " %s" , keyFileName )!=1 ) ParseLineError( cmndCtr , "failed to read in key file name\n" );
			keyFileData->first.read( keyFileName );
			keyFileData->second.set( keyFileData->first , transformType );
			keyFileData->second.setCurrentValue( 0 );
			_keyFileData.push_back( keyFileData );
		}

		//Next we try read in the shader. We make sure that we have
		//not begun describing the scene graph and we make sure that all the
		//file I/O works as it should.
		else if( !strcmp( keyword , "shader" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			if( shaderFlag ) fprintf( stderr , "Parsing Line %d: Ignoring extra shader\n" , cmndCtr );
			{
				shader = new RayShader();
				if( !shader ) ParseLineError( cmndCtr , "could not allocate RayShader\n" );
				shader->read( fp );
				shaderFlag = true;
			}
		}

		//Next we try read out the scene graph information, obtaining the 
		//transform and either creating a new scene graph or adding a node
		//to the existing one.
		else if( !strcmp( keyword , "group_begin" ) ) inShapes = true , ParseGroup( fp , cmndCtr , root );
		else if( !strcmp( keyword , "joint_begin" ) ) inShapes = true , ParseJoint( fp , cmndCtr , root );
		else if( !strcmp( keyword , "cluster_begin" ) ) inShapes = true , ParseCluster( fp , cmndCtr , root );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) inShapes = true , ParseTriangleList( fp , cmndCtr , root );
		
		//We check if we have come to the end of a scene-graph group. If we have
		//this is bad because we have not yet opened one
		else if( !strcmp( keyword , "group_end" ) ) ParseLineError( cmndCtr , "closed more groups than began\n" );
		else if( !strcmp( keyword , "joint_end" ) ) ParseLineError( cmndCtr , "closed more joints than began\n" );
		else if( !strcmp( keyword , "cluster_end" ) ) ParseLineError( cmndCtr , "closed more clusters than began\n" );
		else if( !strcmp( keyword , "shape_triangles_end" ) ) ParseLineError( cmndCtr , "closed more triangle lists than began\n" );
		else if( ParseShape( fp , keyword , cmndCtr , root ) ) inShapes = true;
		else fprintf( stderr , "Ignoring Command %d: No valid #directive... \"%s\"\n" , cmndCtr , keyword );
	}

	for( int i=0 ; i<_materials.size() ; i++ )
	{
		int tIdx = (int)(size_t)_materials[i]->tex;
		if( tIdx==-1 ) _materials[i]->tex = NULL;
		else if( tIdx>=_textures.size() ) ParseLineError( cmndCtr , "material specifies a texture out of texture bounds\n" );
		else _materials[i]->tex = _textures[tIdx];
	}
}

void RayScene::ParseCluster( FILE* fp , int& cmndCtr , RayGroup* current , std::vector< RayVertex >* vList )
{
	char keyword[BUF_SIZE];
	RayGroup *currNode , *newNode;
	currNode = current;
	newNode = new TrivialRayGroup();
	if( !newNode ) ParseLineError( cmndCtr , "failed to allocate memory for TrivialRayGroup\n" );
	currNode->addShape( newNode );
	currNode = newNode;

	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		if     ( !strcmp( keyword , "group_begin" ) ) ParseGroup( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseJoint( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "cluster_begin" ) ) ParseCluster( fp , cmndCtr , currNode , vList );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseTriangleList( fp , cmndCtr , currNode );

		/* We check if we have come to the end of a scene-graph group and we
		* do the appropriate pointer manipulation, making sure that we are not
		* leaving a group we had not begun */
		else if( !strcmp( keyword , "cluster_end" ) ) return;
		else if( !strcmp( keyword , "group_end") ) ParseLineError( cmndCtr , "group_end within a cluster group\n" );
		else if( !strcmp( keyword , "joint_end") ) ParseLineError( cmndCtr , "joint_end within a cluster group\n" );
		else if( !strcmp( keyword , "shape_triangles_end") ) ParseLineError( cmndCtr , "shape_triangles_end within a cluster group\n" );
		else if( ParseShape( fp , keyword , cmndCtr , currNode , vList ) ){}
		else fprintf( stderr , "Ignoring Command %d: No valid #directive... \"%s\" in cluster\n" , cmndCtr , keyword );
	}
	ParseLineError( cmndCtr , "no cluster_end\n" );
}
void RayScene::ParseTriangleList( FILE* fp , int& cmndCtr , RayGroup* current )
{
	bool inShapes = false;
	char keyword[BUF_SIZE];
	int idx;
	RayGroup *currNode;
	TriangleListRayGroup *newNode;
	currNode = current;
	newNode = new TriangleListRayGroup();
	if( !newNode ) ParseLineError( cmndCtr , "failed to allocate memory for StaticGroup\n" );
	newNode->read( fp , &idx );
	if( idx>=_materials.size() ) ParseLineError( cmndCtr , "shape specifies a material that is out of bounds\n" );
	else if( idx<0 ) ParseLineError( cmndCtr , "you cannot have a negative material index\n" );
	else newNode->material = _materials[idx];

	currNode->addShape( newNode );
	currNode = newNode;

	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;

		if( !strcmp( keyword , "vertex" ) )
		{
			if( inShapes ) ParseLineError( cmndCtr , "not a valid directive within a group\n" );
			RayVertex rayVertex;
			rayVertex.read( fp );
			rayVertex.index = (int)newNode->vertices.size();
			newNode->vertices.push_back( rayVertex );
		}

		else if( !strcmp( keyword , "group_begin" ) ) ParseLineError( cmndCtr , "cannot have a static group within a triangle list\n" );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseLineError( cmndCtr , "cannot have a parametrized group within a triangle list\n" );
		else if( !strcmp( keyword , "cluster_begin" ) ) inShapes = true , ParseCluster( fp , cmndCtr , currNode , &newNode->vertices );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseLineError( cmndCtr , "cannot have a triangle list within a triangle list\n" );

		/* We check if we have come to the end of a scene-graph group and we
		* do the appropriate pointer manipulation, making sure that we are not
		* leaving a group we had not begun */
		else if( !strcmp( keyword , "shape_triangles_end" ) ) return;
		else if( !strcmp( keyword , "cluster_end" ) ) ParseLineError( cmndCtr , "group_end within a triangle list\n" );
		else if( !strcmp( keyword , "group_end") ) ParseLineError( cmndCtr , "group_end within a triangle list\n" );
		else if( !strcmp( keyword , "joint_end") ) ParseLineError( cmndCtr , "joint_end within a triangle list\n" );
		else if( ParseShape( fp , keyword , cmndCtr , currNode , &newNode->vertices ) ){}
		else fprintf( stderr , "Ignoring Command %d: No valid #directive... \"%s\" in triangle list\n" , cmndCtr , keyword );
	}
	ParseLineError( cmndCtr , "no shape_triangles_end\n" );
}

void RayScene::ParseGroup( FILE* fp , int& cmndCtr , RayGroup* current )
{
	char keyword[BUF_SIZE];
	RayGroup *currNode;
	StaticRayGroup *newNode;
	currNode = current;
	newNode = new StaticRayGroup();
	if( !newNode ) ParseLineError( cmndCtr , "failed to allocate StaticGroup\n" );
	newNode->read( fp );
	currNode->addShape( newNode );
	currNode = newNode;
	
	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		if     ( !strcmp( keyword , "group_begin" ) ) ParseGroup( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseJoint( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "cluster_begin" ) ) ParseCluster( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseTriangleList( fp , cmndCtr , currNode );
		
		/* We check if we have come to the end of a scene-graph group and we
		* do the appropriate pointer manipulation, making sure that we are not
		* leaving a group we had not begun */
		else if( !strcmp( keyword , "group_end" ) ) return;
		else if( !strcmp( keyword , "joint_end" ) ) ParseLineError( cmndCtr , "joint_end within a group\n" );
		else if( !strcmp( keyword , "cluster_end" ) ) ParseLineError( cmndCtr , "cluster_end within a static group\n" );
		else if( !strcmp( keyword , "shape_triangles_end" ) ) ParseLineError( cmndCtr , "shape_triangles_end within a static group\n" );
		else if( ParseShape( fp , keyword , cmndCtr , currNode ) ){}
		else fprintf( stderr , "Ignoring Command %d: No valid #directive... \"%s\" in static group\n" , cmndCtr , keyword );
	}
	ParseLineError( cmndCtr , "no group_end\n" );
}
void RayScene::ParseJoint( FILE* fp , int& cmndCtr , RayGroup* current )
{
	char paramName[BUF_SIZE];
	char keyword[BUF_SIZE];

	ParametrizedRayGroup *newNode;
	RayGroup *currNode;
	int keyFileDataIndex , paramIndex = -1;
	
	currNode = current;
	if( !fscanf( fp , " %s " , paramName ) ) ParseLineError( cmndCtr , "failed to read parameter name\n" );
	for( keyFileDataIndex=0 ; keyFileDataIndex<_keyFileData.size() ; keyFileDataIndex++ )
	{
		paramIndex = _keyFileData[ keyFileDataIndex ]->first.index( paramName );
		if( paramIndex>=0 ) break;
	}
	if( keyFileDataIndex==_keyFileData.size() ) ParseLineError( cmndCtr , "failed to find joint name in key files\n" );
	newNode = _keyFileData[ keyFileDataIndex ]->second.getParametrizedRayGroup( paramIndex );
	if( !newNode ) ParseLineError( cmndCtr , "failed to link joint node to parameter\n" );
	strcpy( newNode->pName , paramName );
	currNode->addShape( newNode );
	currNode = newNode;
	
	while( fscanf( fp , " #%s" , keyword )==1 )
	{
		cmndCtr++;
		if     ( !strcmp( keyword , "group_begin" ) ) ParseGroup( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_begin" ) ) ParseJoint( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "cluster_begin" ) ) ParseCluster( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "shape_triangles_begin" ) ) ParseTriangleList( fp , cmndCtr , currNode );
		else if( !strcmp( keyword , "joint_end" ) ) return;
		else if( !strcmp( keyword , "cluster_end" ) ) ParseLineError( cmndCtr , "cluster_end within a joint\n" );
		else if( !strcmp( keyword , "shape_triangles_end" ) ) ParseLineError( cmndCtr , "shape_triangles_end within a joint\n" );
		else if( !strcmp( keyword , "group_end" ) ) ParseLineError( cmndCtr , "group_end within a joint\n" );
		else if( ParseShape( fp , keyword , cmndCtr , currNode ) ){}
		else fprintf( stderr , "Ignoring Command %d: No valid #directive... \"%s\" in parametrized group\n" , cmndCtr , keyword );
	}
	ParseLineError( cmndCtr , "no joint_end\n" );
}
bool RayScene::ParseShape( FILE* fp , const char* keyword , int cmndCtr , RayGroup* curr , std::vector< RayVertex >* vList )
{
	RayGroup* current=curr;
	
	/* If we are reading in new files we want them added to the current node
	* and we want to store there information in a temporary scene object we
	* will conveniently forget to free up. */
	if( !vList && !strcmp( keyword , "ray_file_instance" ) )
	{
		int idx;
		if( fscanf( fp , " %d" , &idx)!=1 ) ParseLineError( cmndCtr , "could not read ray_file index number\n" );
		if( idx<0 ) ParseLineError( cmndCtr , "you cannot have negative ray_file index\n" );
		if( idx>=_rayFiles.size() ) ParseLineError( cmndCtr , "shape specifies a ray_file that is out of bounds\n" );
		RayFileInstance* rayFileInstance = new RayFileInstance( _rayFiles[idx] );
		if( !rayFileInstance ) ParseLineError( cmndCtr , "failed to allocate memory for Shape\n" );
		current->addShape( rayFileInstance );
		return true;
	}

	/* We start reading in the shapes. We check that we are inside
	* a group and we call the appropriate instantiator */	

	else if( !vList && !strcmp( keyword , "shape_sphere" ) )
	{
		int idx;
		RaySphere* sphere = new RaySphere();
		if( !sphere ) ParseLineError( cmndCtr , "failed to allocate memory for Sphere\n" );
		sphere->read( fp , &idx );
		if( idx>=_materials.size() ) ParseLineError( cmndCtr , "shape specifies a material that is out of bounds\n" );
		else if( idx<0 ) ParseLineError( cmndCtr , "you cannot have a negative material index\n" );
		else sphere->material = _materials[idx];
		current->addShape( sphere );
		return true;
	}
	
	else if( !vList && !strcmp( keyword , "shape_box" ) )
	{
		int idx;
		RayBox* box = new RayBox();
		if( !box ) ParseLineError( cmndCtr , "failed to allocate memory for Box\n" );
		box->read( fp , &idx );
		if( idx>=_materials.size() ) ParseLineError( cmndCtr , "shape specifies a material that is out bounds\n" );
		else if( idx<0 ) ParseLineError( cmndCtr , "you cannot have a negative material index\n" );
		else box->material = _materials[idx];
		current->addShape( box );
		return true;
	}
    
	else if( !vList && !strcmp( keyword , "shape_cone" ) )
	{
		int idx;
		RayCone* cone = new RayCone();
		if( !cone ) ParseLineError( cmndCtr , "failed to allocate memory for Cone\n" );
		cone->read( fp , &idx );
		if( idx>=_materials.size() ) ParseLineError( cmndCtr , "shape specifies a material that is out of bounds\n" );
		else if( idx<0 ) ParseLineError( cmndCtr , "you cannot have a negative material index\n" );
		else cone->material = _materials[idx];
		current->addShape( cone );
		return true;
	}
	
	else if( !vList && !strcmp( keyword , "shape_cylinder" ) )
	{
		int idx;
		RayCylinder* cylinder = new RayCylinder();
		if( !cylinder ) ParseLineError( cmndCtr , "failed to allocate memory for Cylinder\n" );
		cylinder->read( fp , &idx );
		if( idx>=_materials.size() ) ParseLineError( cmndCtr , "shape specifies a material that is out of bounds\n" );
		else if( idx<0 ) ParseLineError( cmndCtr , "you cannot have a negative material index\n" );
		else cylinder->material = _materials[idx];
		current->addShape( cylinder );
		return true;
	}
	else if( vList && !strcmp( keyword , "shape_triangle" ) )
	{
		RayTriangle* triangle = new RayTriangle();
		if( !triangle ) ParseLineError( cmndCtr , "failed to allocate memory for Triangle\n" );
		triangle->read( fp , *vList );
		current->addShape( triangle );
		return true;
	}
	return false;
}		

void RayScene::setUpOpenGL( int cplx )
{
	if( shader && shader->glslProgram ) shader->glslProgram->setup();
	for( int i=0 ; i<_textures.size() ; i++ ) _textures[i]->setUpOpenGL();
	for( int i=0 ; i<_rayFiles.size() ; i++ ) _rayFiles[i]->scene->setUpOpenGL( cplx );
	group->setUpOpenGL( cplx , 1 );
}

void RayScene::drawOpenGL( void )
{
	camera->drawOpenGL();

#ifndef NO_OPEN_GL
	if( shader && shader->glslProgram ) shader->glslProgram->use();
	else glUseProgram( 0 );

	glEnable( GL_LIGHTING );
#endif // NO_OPEN_GL
	for( int i=0 ; i<lights.size() ; i++ ) lights[i]->drawOpenGL( i , shader ? shader->glslProgram : NULL );
	group->drawOpenGL( -1 , shader ? shader->glslProgram : NULL );
}
void RayScene::setCurrentTime( double t , int curveFit )
{
	for( int i=0 ; i<_keyFileData.size() ; i++ )
	{
		double tt=t/_keyFileData[i]->first.duration;
		tt -= (int)tt;
		_keyFileData[i]->second.setCurrentValue( tt , curveFit );
	}
	for( int i=0 ; i<_rayFiles.size() ; i++ ) _rayFiles[i]->scene->setCurrentTime( t , curveFit );
}

Image32 RayScene::rayTrace( int width , int height , int rLimit , double cLimit )
{
	Image32 img;
	Ray3D ray;
	Pixel32 p;

	img.setSize( width , height );
	ray.position = camera->position;
	for( int i=0 ; i<width ; i++ )
	{
		printf( "           \r" );
		printf( "%3.1f\r" , (float)i/width*100 );
		for( int j=0 ; j<height ; j++ )
		{
			ray = GetRay( camera , i , height-j-1 , width , height );
			Point3D c = getColor( ray , rLimit , Point3D( cLimit , cLimit , cLimit ) );
			p.r = (int)(c[0]*255);
			p.g = (int)(c[1]*255);
			p.b = (int)(c[2]*255);
			img(i,j)=p;
		}
	}
	return img;
}

void RayScene::ParseError( const char *s )
{
	throw RayException( std::string( "Parsing error: " ) + s );
}

void RayScene::ParseLineError( int i , const char *s )
{
	throw RayException( std::string( "Parsing error of command " ) + std::to_string( i ) + ": " + s );
}
