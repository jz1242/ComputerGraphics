#include <stdio.h>
#include <string.h>
#include <Util/exceptions.h>
#include <Util/geometry.h>
#include <Util/cmdLineParser.h>
#include "rayGroup.h"
#include "rayKey.h"

////////////////
// RayKeyFile //
////////////////
RayKeyFile::RayKeyFile( void ){}
RayKeyFile::RayKeyFile( const RayKeyFile& rayKeyFile )
{
	_keyTransforms = rayKeyFile._keyTransforms;
	_pNames.resize( rayKeyFile._pNames.size() );
	for( int i=0 ; i<_pNames.size() ; i++ )
	{
		_pNames[i] = new char[ strlen( rayKeyFile._pNames[i] )+1 ];
		strcpy( _pNames[i] , rayKeyFile._pNames[i] );
	}
}
RayKeyFile& RayKeyFile::operator = ( const RayKeyFile& rayKeyFile )
{
	for( int i=0 ; i<_pNames.size() ; i++ ) if( _pNames[i] ) delete[] _pNames[i];
	_pNames.resize(0);
	_keyTransforms = rayKeyFile._keyTransforms;
	_pNames.resize( rayKeyFile._pNames.size() );
	for( int i=0 ; i<_pNames.size() ; i++ )
	{
		_pNames[i] = new char[ strlen( rayKeyFile._pNames[i] )+1 ];
		strcpy( _pNames[i] , rayKeyFile._pNames[i] );
	}
	return *this;
}
RayKeyFile::~RayKeyFile( void )
{
	for( int i=0 ; i<_pNames.size() ; i++ ) if( _pNames[i] ) delete[] _pNames[i];
	_pNames.resize(0);
}
std::vector< Matrix4D >& RayKeyFile::operator[] ( int index ){ return _keyTransforms[index]; }
const char* RayKeyFile::name( int idx ){ return _pNames[idx]; }
int RayKeyFile::index( char* name )
{
	for( int i=0 ; i<_pNames.size() ; i++ ) if( !strcasecmp( name , _pNames[i] ) ) return i;
	return -1;
}

void RayKeyFile::read( const char *fileName )
{
	char temp[100];

	FILE* fp = fopen( fileName , "r" );
	if( !fp ) Util::Throw( "Failed to open file for reading: %s" , fileName );

	int dofs;
	if( fscanf( fp , " #DOFS %d " , &dofs )!=1 || dofs<=0 ){ fclose(fp) ; Util::Throw( "Failed to parse DOFS" ); }
	_pNames.resize( dofs );
	_keyTransforms.resize( dofs );
	for( int i=0 ; i<dofs ; i++ )
	{
		if( fscanf( fp , " %s " , temp )!=1 ){ fclose(fp) ; Util::Throw( "Failed to read DOFS" ); }
		_pNames[i] = new char[strlen(temp)+1];
		if( !_pNames[i] )
		{
			for( int j=0 ; j<i ; j++ ) delete[] _pNames[j];
			_pNames.resize( 0 );
			fclose( fp );
			Util::Throw( "Failed to allocate parameter names" );
		}
		strcpy( _pNames[i] , temp );
	}
	int frames;
	if( fscanf( fp , " #DURATION %f " , &duration )!=1 || duration<=0 || fscanf( fp , " #FRAMES %d " , &frames )!=1 || frames<=0 )
	{
		for( int i=0 ; i<_pNames.size() ; i++ ) delete[] _pNames[i];
		_pNames.resize( 0 );
		fclose(fp);
		Util::Throw( "Failed to read DURATION" );
	}
	for( int i=0 ; i<_keyTransforms.size() ; i++ ) _keyTransforms[i].resize( frames );

	for( int j=0 ; j<frames ; j++ ) for( int i=0 ; i<dofs ; i++ ) for( int k=0 ; k<4 ; k++ ) for( int l=0 ; l<4 ; l++ )
	{
		float t;
		if( fscanf( fp , " %f" , &t )!=1 )
		{
			for( int i=0 ; i<_pNames.size() ; i++ ) delete[] _pNames[i];
			_pNames.resize( 0 );
			fclose( fp );
			Util::Throw( "Failed to read parameter" );
		}
		_keyTransforms[i][j](k,l) = t;
	}
	fclose( fp );
}
void RayKeyFile::write( const char *fileName )
{
	FILE* fp = fopen( fileName , "w" );
	if( !fp ) Util::Throw( "Failed to open file for reading: %s" , fileName );

	fprintf( fp , " #DOFS %d " , (int)_pNames.size() );
	for( int i=0 ; i<_pNames.size() ; i++ ) fprintf( fp , "\t%s\n" , _pNames[i] );
	fprintf( fp , " #DURATION %f\n" , duration );
	fprintf( fp , " #FRAMES %d\n" , (int)_keyTransforms[0].size() );
	for( int i=0 ; i<_pNames.size() ; i++ )
	{
		fprintf( fp , "\t" );
		for( int j=0 ; j<_keyTransforms[i].size() ; j++ )
			for( int k=0 ; k<4 ; k++ ) for( int l=0 ; l<4 ; l++ ) fprintf( fp , " %f" , _keyTransforms[i][j](k,l) );
		fprintf( fp , "\n" );
	}
	fclose( fp );
}
int RayKeyFile::samples( void ) const { return _keyTransforms.size()>0 ? (int)_keyTransforms[0].size() : 0; }
int RayKeyFile::parameters( void ) const { return (int)_pNames.size(); }

////////////////
// RayKeyData //
////////////////
const char* RayKeyData::RepresentationNames[] = { "matrix" , "euler and translation" , "closest rotation and translation" , "rotation logarithm and translation" , "quaternion and translation" };

RayKeyData::RayKeyData( void ) : _type(-1) {}
RayKeyData::~RayKeyData( void ) { _clear(); }
void RayKeyData::_clear( void )
{
	_sampleData.resize(0);
	_type = -1;
}

void RayKeyData::setCurrentValue( double t , int curveType )
{
	for( int i=0 ; i<_sampleData.size() ; i++ ) _sampleData[i]->setCurrentValue( t , curveType );
}
ParametrizedRayGroup* RayKeyData::getParametrizedRayGroup( int i )
{
	ParametrizedMatrix4D *matrix;
	ParametrizedEulerAnglesAndTranslation* eulerAndTranslation;
	ParametrizedClosestRotationAndTranslation* rClosestAndTranslation;
	ParametrizedRotationLogarithmAndTranslation* rLogarithmAndTranslation;
	ParametrizedQuaternionAndTranslation* quaternionAndTranslation;
	if( i<0 || i>=_sampleData.size() ) return NULL;

	switch( _type )
	{
	case REPRESENTATION_MATRIX:
		matrix = new ParametrizedMatrix4D();
		if( !matrix ) return NULL;
		matrix->value = &( ( ( ParameterSamples< Matrix4D >* )_sampleData[i] )->currentValue );
		return matrix;
	case REPRESENTATION_EULER_AND_TRANSLATION:
		eulerAndTranslation = new ParametrizedEulerAnglesAndTranslation();
		if( !eulerAndTranslation ) return NULL;
		eulerAndTranslation->value = &( ( ( ParameterSamples< EulerAnglesAndTranslation >* )_sampleData[i] )->currentValue );
		return eulerAndTranslation;
	case REPRESENTATION_CLOSEST_ROTATION_AND_TRANSLATION:
		rClosestAndTranslation = new ParametrizedClosestRotationAndTranslation();
		if( !rClosestAndTranslation ) return NULL;
		rClosestAndTranslation->value = &( ( ( ParameterSamples< RotationAndTranslation >* )_sampleData[i] )->currentValue );
		return rClosestAndTranslation;
	case REPRESENTATION_ROTATION_LOGARITHM_AND_TRANSLATION:
		rLogarithmAndTranslation = new ParametrizedRotationLogarithmAndTranslation();
		if( !rLogarithmAndTranslation ) return NULL;
		rLogarithmAndTranslation->value = &( ( ( ParameterSamples< LogRotationAndTranslation >* )_sampleData[i] )->currentValue );
		return rLogarithmAndTranslation;
	case REPRESENTATION_QUATERNION_AND_TRANSLATION:
		quaternionAndTranslation = new ParametrizedQuaternionAndTranslation();
		if( !quaternionAndTranslation ) return NULL;
		quaternionAndTranslation->value = &( ( ( ParameterSamples< QuaternionAndTranslation >* )_sampleData[i] )->currentValue );
		return quaternionAndTranslation;
	}
	return NULL;
}

void RayKeyData::set( RayKeyFile& keyFile , int tp )
{
	_type = tp;
	_sampleData.resize( keyFile.parameters() );

	for( int i=0 ; i<_sampleData.size() ; i++ ) _sampleData[i] = NULL;
	for( int i=0 ; i<_sampleData.size() ; i++ )
	{
		switch( _type )
		{
		case REPRESENTATION_MATRIX:                             _sampleData[i] = new ParameterSamples< Matrix4D >()                  ; break;
		case REPRESENTATION_EULER_AND_TRANSLATION:              _sampleData[i] = new ParameterSamples< EulerAnglesAndTranslation >() ; break;
		case REPRESENTATION_CLOSEST_ROTATION_AND_TRANSLATION:   _sampleData[i] = new ParameterSamples< RotationAndTranslation >()    ; break;
		case REPRESENTATION_ROTATION_LOGARITHM_AND_TRANSLATION: _sampleData[i] = new ParameterSamples< LogRotationAndTranslation >() ; break;
		case REPRESENTATION_QUATERNION_AND_TRANSLATION:         _sampleData[i] = new ParameterSamples< QuaternionAndTranslation >()  ; break;
		};
		if( !_sampleData[i] || !_sampleData[i]->set( keyFile.samples() ) ){ _clear() ; Util::Throw( "Failed to set sample data" ); }
		for( int j=0 ; j<keyFile.samples() ; j++ )
		{
			switch( _type )
			{
			case REPRESENTATION_MATRIX:
				( ( ParameterSamples< Matrix4D >* )_sampleData[i] )->samples[j] = keyFile[i][j];
				break;
			case REPRESENTATION_EULER_AND_TRANSLATION:
				if( j ) ( ( ParameterSamples< EulerAnglesAndTranslation >* )_sampleData[i] )->samples[j] = EulerAnglesAndTranslation( keyFile[i][j] , ( ( ParameterSamples< EulerAnglesAndTranslation >* )_sampleData[i] )->samples[j-1] );
				else    ( ( ParameterSamples< EulerAnglesAndTranslation >* )_sampleData[i] )->samples[j] = keyFile[i][j];
				break;
			case REPRESENTATION_CLOSEST_ROTATION_AND_TRANSLATION:
				( ( ParameterSamples< RotationAndTranslation >* )_sampleData[i] )->samples[j] = keyFile[i][j];
				break;
			case REPRESENTATION_ROTATION_LOGARITHM_AND_TRANSLATION:
				if( j ) ( ( ParameterSamples< LogRotationAndTranslation >* )_sampleData[i] )->samples[j] = LogRotationAndTranslation( keyFile[i][j] , ( ( ParameterSamples< LogRotationAndTranslation >* )_sampleData[i] )->samples[j-1] );
				else    ( ( ParameterSamples< LogRotationAndTranslation >* )_sampleData[i] )->samples[j] = keyFile[i][j];
				break;
			case REPRESENTATION_QUATERNION_AND_TRANSLATION:
				if( j ) ( ( ParameterSamples< QuaternionAndTranslation >* )_sampleData[i] )->samples[j] = QuaternionAndTranslation( keyFile[i][j] , ( ( ParameterSamples< QuaternionAndTranslation >* )_sampleData[i] )->samples[j-1] );
				else    ( ( ParameterSamples< QuaternionAndTranslation >* )_sampleData[i] )->samples[j] = keyFile[i][j];
				break;
			};
		}
	}
}
