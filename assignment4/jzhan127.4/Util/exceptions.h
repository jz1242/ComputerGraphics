#ifndef EXCEPTIONS_INCLUDED
#define EXCEPTIONS_INCLUDED

#include <stdarg.h>
#include <exception>
#include <string>
#include <string.h>

#define VERBOSE_MESSAGING

namespace Util
{
#ifdef VERBOSE_MESSAGING
	inline char *_MakeMessageString( const char *header , const char *fileName , int line , const char *functionName , const char *format , ... )
	{
		va_list args;
		va_start( args , format );

		// Formatting is:
		// <header> <filename> (Line <line>)
		// <header size> <function name>
		// <header size> <format message>
		char lineBuffer[25];
		sprintf( lineBuffer , "(Line %d)" , line );
		size_t _size , size=0;

		// Line 1
		size += strlen(header)+1;
		size += strlen(fileName)+1;
		size += strlen(lineBuffer)+1;

		// Line 2
		size += strlen(header)+1;
		size += strlen(functionName)+1;

		// Line 3
		size += strlen(header)+1;
		size += vsnprintf( NULL , 0 , format , args );

		char *_buffer , *buffer = new char[ size+1 ];
		_size = size , _buffer = buffer;

		// Line 1
		sprintf( _buffer , "%s " , header );
		_buffer += strlen(header)+1;
		_size -= strlen(header)+1;

		sprintf( _buffer , "%s " , fileName );
		_buffer += strlen(fileName)+1;
		_size -= strlen(fileName)+1;

		sprintf( _buffer , "%s\n" , lineBuffer );
		_buffer += strlen(lineBuffer)+1;
		_size -= strlen(lineBuffer)+1;

		// Line 2
		for( int i=0 ; i<strlen(header)+1 ; i++ ) _buffer[i] = ' ';
		_buffer += strlen(header)+1;
		_size -= strlen(header)+1;

		sprintf( _buffer , "%s\n" , functionName );
		_buffer += strlen(functionName)+1;
		_size -= strlen(functionName)+1;

		// Line 3
		for( int i=0 ; i<strlen(header)+1 ; i++ ) _buffer[i] = ' ';
		_buffer += strlen(header)+1;
		_size -= strlen(header)+1;

		vsnprintf( _buffer , _size+1 , format , args );

		return buffer;
	}

	struct Exception : public std::exception
	{
		const char *what( void ) const noexcept { return _message.c_str(); }
		template< typename ... Args >
		Exception( const char *fileName , int line , const char *functionName , const char *format , Args ... args )
		{
			char *buffer = _MakeMessageString( "[EXCEPTION]" , fileName , line , functionName , format , args ... );
			_message = std::string( buffer );
			delete[] buffer;
		}
	protected:
		std::string _message;
	};

	template< typename ... Args > void _Throw( const char *fileName , int line , const char *functionName , const char *format , Args ... args ){ throw Exception( fileName , line , functionName , format , args ... ); }
	template< typename ... Args >
	void _Warn( const char *fileName , int line , const char *functionName , const char *format , Args ... args )
	{
		char *buffer = _MakeMessageString( "[WARNING]" , fileName , line , functionName , format , args ... );
		fprintf( stderr , "%s\n" , buffer );
		delete[] buffer;
	}
	template< typename ... Args >
	void _ErrorOut( const char *fileName , int line , const char *functionName , const char *format , Args ... args )
	{
		char *buffer = _MakeMessageString( "[ERROR]" , fileName , line , functionName , format , args ... );
		fprintf( stderr , "%s\n" , buffer );
		delete[] buffer;
	}
#ifndef Warn
#define Warn( ... ) _Warn( __FILE__ , __LINE__ , __FUNCTION__ , __VA_ARGS__ )
#endif // Warn
#ifndef Throw
#define Throw( ... ) _Throw( __FILE__ , __LINE__ , __FUNCTION__ , __VA_ARGS__ )
#endif // Throw
#ifndef ErrorOut
#define ErrorOut( ... ) _ErrorOut( __FILE__ , __LINE__ , __FUNCTION__ , __VA_ARGS__ )
#endif // ErrorOut

#else // !VERBOSE_MESSAGING
	inline char *_MakeMessageString( const char *header , const char *functionName , const char *format , ... )
	{
		va_list args;
		va_start( args , format );

		size_t _size , size = vsnprintf( NULL , 0 , format , args );
		size += strlen(header)+1;
		size += strlen(functionName)+2;

		char *_buffer , *buffer = new char[ size+1 ];
		_size = size , _buffer = buffer;

		sprintf( _buffer , "%s " , header );
		_buffer += strlen(header)+1;
		_size -= strlen(header)+1;

		sprintf( _buffer , "%s: " , functionName );
		_buffer += strlen(functionName)+2;
		_size -= strlen(functionName)+2;

		vsnprintf( _buffer , _size+1 , format , args );

		return buffer;
	}
	struct Exception : public std::exception
	{
		const char *what( void ) const noexcept { return _message.c_str(); }
		template< typename ... Args >
		Exception( const char *functionName , const char *format , Args ... args )
		{
			char *buffer = _MakeMessageString( "[EXCEPTION]" , functionName , format , args ... );
			_message = std::string( buffer );
			delete[] buffer;
		}
	protected:
		std::string _message;
	};
	template< typename ... Args > void _Throw( const char *functionName , const char *format , Args ... args ){ throw Exception( functionName , format , args ... ); }
	template< typename ... Args >
	void _Warn( const char *functionName , const char *format , Args ... args )
	{
		char *buffer = _MakeMessageString( "[WARNING]" , functionName , format , args ... );
		fprintf( stderr , "%s\n" , buffer );
		delete[] buffer;
	}
	template< typename ... Args >
	void _ErrorOut( const char *functionName , const char *format , Args ... args )
	{
		char *buffer = _MakeMessageString( "[ERROR]" , functionName , format , args ... );
		fprintf( stderr , "%s\n" , buffer );
		delete[] buffer;
	}
#ifndef Warn
#define Warn( ... ) _Warn( __FUNCTION__ , __VA_ARGS__ )
#endif // Warn
#ifndef Throw
#define Throw( ... ) _Throw( __FUNCTION__ , __VA_ARGS__ )
#endif // Throw
#ifndef ErrorOut
#define ErrorOut( ... ) _ErrorOut( __FUNCTION__ , __VA_ARGS__ )
#endif // ErrorOut
#endif // VERBOSE_MESSAGING
}
#endif // EXCEPTIONS_INCLUDED
