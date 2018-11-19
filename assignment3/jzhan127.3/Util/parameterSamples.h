#ifndef PARAMETER_SAMPLES_INCLUDED 
#define PARAMETER_SAMPLES_INCLUDED

enum
{
	INTERPOLATION_LINEAR,
	INTERPOLATION_CATMULL_ROM,
	INTERPOLATION_UNIFORM_CUBIC_B_SPLINE ,
	INTERPOLATION_COUNT
};
static const char* InterpolationNames[] =
{
	"Linear" , "Catmull-Rom" , "Uniform Cubic B-Spline"
};
/** This abstract class represents a variable whose value is obtained by interpolating sample data. */
class VParameterSamples
{
public:
	/** This virtual method initializes the array of data */
	virtual bool set( int count )=0;
	/** This method updates the value of the in-between value, by computing the interpolation/approximation at value 0<=t<=1.
	  * The value type specifies how the interpolation/approximation is to be done.*/
	virtual void setCurrentValue( double t , int type=INTERPOLATION_LINEAR )=0;
};
/** This template class stores an array of samples of a parameter. It is used for computing
  * the  in-between values using linear interpolation or spline methods.
  * In order to be used, the template class needs to support addition and right
  * multiplication by a number. */
template< class Vector >
class ParameterSamples : public VParameterSamples
{
public:
	/** The sample values */
	std::vector< Vector > samples;
	/** The computed in-between value */
	Vector currentValue;

	/** This method initializes the array of data*/
	bool set( int count );
	/** This overloaded operator returns a reference to the i-th sample value */
	Vector& operator[] ( int i );
	/** This overloaded operator returns a reference to the i-th sample value */
	const Vector& operator[] ( int i ) const;
	/** This method updates the value of the in-between value, by computing the interpolation/approximation at value 0<=t<=1.
	  * The value type specifies how the interpolation/approximation is to be done.*/
	void setCurrentValue( double t , int type=INTERPOLATION_LINEAR );
};

template< class Vector >
bool ParameterSamples< Vector >::set( int c )
{
	samples.resize( c>0 ? c : 0 );
	return true;
}

template< class Vector >
Vector& ParameterSamples< Vector >::operator [] ( int i ) { return samples[i]; }
template< class Vector >
const Vector& ParameterSamples< Vector >::operator [] ( int i ) const { return samples[i]; }

#include "parameterSamples.todo.inl"

#endif //PARAMETER_SAMPLES_INCLUDED
