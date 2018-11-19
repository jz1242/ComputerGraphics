#include "lineSegments.h"
#include <math.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
float OrientedLineSegment::length( void ) const
{
	throw LineSegmentsException( "OrientedLineSegment::length undefined" );
	return 0;
}
float OrientedLineSegment::distance( int x , int y ) const
{
	throw LineSegmentsException( "OrientedLineSegment::distance undefined" );
	return 0;
}
void OrientedLineSegment::getPerpendicular( float& x , float &y ) const
{
	throw LineSegmentsException( "OrientedLineSegment::getPerpendicular undefined" );
}

void OrientedLineSegment::GetSourcePosition( const OrientedLineSegment& source , const OrientedLineSegment& destination , int targetX , int targetY , float& sourceX , float& sourceY )
{
	throw LineSegmentsException( "OrientedLineSegment::GetSourcePosition undefined" );
}
