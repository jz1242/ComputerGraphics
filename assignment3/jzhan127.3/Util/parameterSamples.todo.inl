#include <math.h>

/////////////////////
// Animation stuff //
/////////////////////
template < class Vector >
void ParameterSamples< Vector >::setCurrentValue( double t , int type )
{
	t *= samples.size();
	int it1 = (int)floor( t );
	int it2 = (it1+1)%samples.size();
	double dt = t - it1;
	switch( type )
	{
		case INTERPOLATION_LINEAR:
			currentValue = samples[0];
			break;
		case INTERPOLATION_CATMULL_ROM:
			currentValue = samples[0];
			break;
		case INTERPOLATION_UNIFORM_CUBIC_B_SPLINE:
			currentValue = samples[0];
			break;
	};
	throw GeometryException( "ParameterSamples::setCurrentValue undefined" );
}
