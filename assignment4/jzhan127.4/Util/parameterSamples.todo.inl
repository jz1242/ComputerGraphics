#include <math.h>
#include <Util/exceptions.h>

/////////////////////
// Animation stuff //
/////////////////////
template < class Vector >
void ParameterSamples< Vector >::setCurrentValue( double t , int type )
{
	t *= samples.size();
	int it1 = (int)floor( t );
	int it2 = (it1+1)%samples.size();
	int it0 = (it1 - 1) % samples.size();
	int it3 = (it2 + 1) % samples.size();
	double dt = t - it1;
	double u = dt;
	double c0, c1, c2, c3, b03, b13, b23, b33;
	double s = 1.0 / 2;
	double third = 1.0 / 3;
	double half = 1.0 / 2;
	double threehalf = 3.0 / 2;
	double sixth = 1.0 / 6;
	double twothird = 2.0 / 3;
	switch( type )
	{
		case INTERPOLATION_LINEAR:
			currentValue = samples[it1]*(1 - dt) + samples[it2]* (dt);
			break;
		case INTERPOLATION_CATMULL_ROM:
			//cardinal s = 1
			c0 = -pow(u, 3) + 2 * pow(u, 2) - u;
			c1 = pow(u, 3) - 2 * pow(u, 2) + 1;
			c2 = -pow(u, 3) + pow(u, 2) + u;
			c3 = pow(u, 3) - pow(u, 2);
			currentValue = samples[it0]*c0 + samples[it1]*c1 + samples[it2]*c2 + samples[it3]*c3;
			break;
		case INTERPOLATION_UNIFORM_CUBIC_B_SPLINE:
			b03 = (third - s)*pow(u, 3) + (-half + 2 * s)*pow(u, 2) - s * u + sixth;
			b13 = (1 - s)*pow(u, 3) + (-threehalf + s)*pow(u, 2) + twothird;
			b23 = (-1 + s)*pow(u, 3) + (threehalf - 2 * s)*pow(u, 2) + s * u + sixth;
			b33 = (-third + s)*pow(u, 3) + (half - s)*pow(u, 2);
			currentValue = samples[it0]*b03 + samples[it1]*b13 + samples[it2]*b23 + samples[it3]*b33;
			break;
	};
}
