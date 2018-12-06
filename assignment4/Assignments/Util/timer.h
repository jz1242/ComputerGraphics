#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

#include <chrono>
namespace Util
{
	struct Timer
	{
		Timer( void ){ _start = std::chrono::high_resolution_clock::now(); }
		double elapsed( void ) const
		{
			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
			std::chrono::duration< double > duration = std::chrono::duration_cast< std::chrono::duration< double > >( now - _start );
			return duration.count();
		};
	protected:
		std::chrono::high_resolution_clock::time_point _start;
	};
}
#endif // TIMER_INCLUDED
