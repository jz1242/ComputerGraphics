#include <math.h>
#include <Util/exceptions.h>

/////////////////////
// Animation stuff //
/////////////////////
template < class Vector >
void ParameterSamples< Vector >::setCurrentValue( double t , int type )
{
	//t *= samples.size();
	//int it1 = (int)floor( t );
	//int it2 = (it1+1)%samples.size();
	//double dt = t - it1;
	//switch( type )
	//{
	//	case INTERPOLATION_LINEAR:
	//		currentValue = samples[0];
	//		break;
	//	case INTERPOLATION_CATMULL_ROM:
	//		currentValue = samples[0];
	//		break;
	//	case INTERPOLATION_UNIFORM_CUBIC_B_SPLINE:
	//		currentValue = samples[0];
	//		break;
	//};
	////Util::Throw( "ParameterSamples::setCurrentValue undefined" 
	size_t size = samples.size();
	t *= samples.size();
	int it1 = (int)floor(t);
	int it2 = (it1 + 1) % samples.size();
	double dt = t - it1;
	type = INTERPOLATION_LINEAR;
	//type = INTERPOLATION_CATMULL_ROM;
	//type = INTERPOLATION_UNIFORM_CUBIC_B_SPLINE;
	switch (type)
	{
	case INTERPOLATION_LINEAR: {
		currentValue = samples[it1] * (1 - dt) + samples[it2] * dt;
		break;
	}
	case INTERPOLATION_CATMULL_ROM: {
		double it0 = (it1 - 1) % size;
		double it3 = (it1 + 2) % size;

		double u = dt;
		double u2 = u * u;
		double u3 = u2 * u;
		double c0 = -u3 + 2 * u2 - u;
		double c1 = u3 - 2 * u2 + 1;
		double c2 = -u3 + u2 + u;
		double c3 = u3 - u2;

		currentValue = samples[it0] * c0 + samples[it1] * c1 + samples[it2] * c2 + samples[it3] * c3;
		break;
	}
	case INTERPOLATION_UNIFORM_CUBIC_B_SPLINE: {
		double it0 = (it1 - 1) % size;
		double it3 = (it1 + 2) % size;

		double s = 0.5;
		double u = dt;
		double u2 = u * u;
		double u3 = u2 * u;

		double b0 = (1.0 / 3 - s) * u3 + (-1.0 / 2 + 2 * s) * u2 - s * u + (1.0 / 6);
		double b1 = (1 - s) * u3 + (-3.0 / 2 + s) * u2 + (2.0 / 3);
		double b2 = (-1 + s) * u3 + (3.0 / 2 - 2 * s) * u2 + s * u + (1.0 / 6);
		double b3 = (-1.0 / 3 + s) * u3 + (1.0 / 2 - s) * u2;

		currentValue = samples[it0] * b0 + samples[it1] * b1 + samples[it2] * b2 + samples[it3] * b3;
		break;
	}
	};
}
