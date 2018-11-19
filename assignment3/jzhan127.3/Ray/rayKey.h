#ifndef KEY_FILE_INCLUDED
#define KEY_FILE_INCLUDED

#include <Util/parameterSamples.h>
#include <vector>

/** This class stores the matrix samples for all the parameters of the key-frames read out from a .key file */
class RayKeyFile
{
	std::vector< char* > _pNames;
	std::vector< std::vector< Matrix4D > > _keyTransforms;
public:
	/** This is the duration (in seconds) over which the animation is to play */
	float duration;
	RayKeyFile( void );
	RayKeyFile( const RayKeyFile& rayKeyFile );
	RayKeyFile& operator = ( const RayKeyFile& rayKeyFile );
	~RayKeyFile( void );

	/** This method reads in the transformation data from the specified file */
	void read( char* fileName );
	/** This method writes out the transformation data to the specified file */
	void write( char* fileName );
	/** This method provides access to the i-th row of transformation information.
	  * The ordering is such that the (i,j)-th entry corresponds to the i-th sample of the j-th parameter.*/
	std::vector< Matrix4D >& operator[] ( int i );
	/** This method returns the name of the i-th parameter */
	const char* name( int i );
	/** This method returns the index of the parameter with the specified name. -1 is returned if no parameter matches the name. */
	int index( char* name );
	/** This method returns the number of key-frames stored. */
	int samples( void ) const;
	/** This method returns the number of parameters stored. */
	int parameters( void ) const;
};

/** This class stores the transformation samples after they have been processed */
class RayKeyData
{
	std::vector< VParameterSamples* > _sampleData;
	int _type;
	void _clear( void );
public:
	enum
	{
		REPRESENTATION_MATRIX ,
		REPRESENTATION_EULER_AND_TRANSLATION ,
		REPRESENTATION_CLOSEST_ROTATION_AND_TRANSLATION ,
		REPRESENTATION_ROTATION_LOGARITHM_AND_TRANSLATION ,
		REPRESENTATION_QUATERNION_AND_TRANSLATION ,
		REPRESENTATION_COUNT
	};
	static const char* RepresentationNames[];

	RayKeyData( void );
	~RayKeyData( void );

	/** This method sets the RayKeyData object from a collection of key-frame transformations, using the specified type to indicate how the transformations
	  * are to be factored */
	void set( RayKeyFile& keyFile , int type=REPRESENTATION_MATRIX );
	/** Tis method updates the in-between value of all the parameters, using the interpolation/approximation method specified by curveType. */
	void setCurrentValue( double t , int curveType=INTERPOLATION_LINEAR );

	/** This method returns a ParametrizedRayGroup object which links to the in-between value of the i-th parameter.*/
	class ParametrizedRayGroup* getParametrizedRayGroup( int i );

};
#endif // KEY_FILE_INCLUDED
