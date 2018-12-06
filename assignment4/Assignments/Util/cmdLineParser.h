/*
Copyright (c) 2006, Michael Kazhdan and Matthew Bolitho
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer. Redistributions in binary form must reproduce
the above copyright notice, this list of conditions and the following disclaimer
in the documentation and/or other materials provided with the distribution. 

Neither the name of the Johns Hopkins University nor the names of its contributors
may be used to endorse or promote products derived from this software without specific
prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.
*/

#ifndef CMD_LINE_PARSER_INCLUDED
#define CMD_LINE_PARSER_INCLUDED
#include <stdarg.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

/** This class represents a named argument that can be read from the command line */
class cmdLineReadable
{
public:
	/** Has the argument been set */
	bool set;
	/**The argument's name */
	char *name;

	/** Constructor with the name of the argument */
	cmdLineReadable( const char *name );
	virtual ~cmdLineReadable( void );
	
	/** Try to set the argument from the list of command line arguments */ 
	virtual int read( char** argv , int argc );
	/** Output the argument (and value) to the string */
	virtual void writeValue( char* str ) const;
};

template< class Type > void cmdLineWriteValue( Type t , char* str );
template< class Type > void cmdLineCleanUp( Type* t );
template< class Type > Type cmdLineInitialize( void );
template< class Type > Type cmdLineCopy( Type t );
template< class Type > Type cmdLineStringToType( const char* str );

/** This templated class represents a named argument of the prescribed type */
template< class Type >
class cmdLineParameter : public cmdLineReadable
{
public:
	/** The value the parameter has been set to */
	Type value;
	/** Constructor with the name of the argument  and the default value */
	cmdLineParameter( const char *name );
	cmdLineParameter( const char *name , Type v );
	~cmdLineParameter( void );

	/** Try to set the argument from the list of command line arguments */ 
	int read( char** argv , int argc );
	/** Output the argument (and value) to the string */
	void writeValue( char* str ) const;
};

/** This templated class represents a named argument taking a fixed number of values of the prescribed type */
template< class Type , int Dim >
class cmdLineParameterArray : public cmdLineReadable
{
public:
	/** The values the parameter has been set to */
	Type values[Dim];
	/** Constructor with the name of the argument and the default values */
	cmdLineParameterArray( const char *name, const Type* v=NULL );
	~cmdLineParameterArray( void );
	/** Try to set the argument from the list of command line arguments */ 
	int read( char** argv , int argc );
	/** Output the argument (and value) to the string */
	void writeValue( char* str ) const;
};

/** This templated class represents a named argument taking a variable number of values of of the prescribed type */
template< class Type >
class cmdLineParameters : public cmdLineReadable
{
public:
	/** The number of values the argument takes */
	int count;
	/** The values the parameter has been set to */
	Type *values;
	cmdLineParameters( const char* name );
	~cmdLineParameters( void );
	/** Try to set the argument from the list of command line arguments */ 
	int read( char** argv , int argc );
	/** Output the argument (and value) to the string */
	void writeValue( char* str ) const;
};

/** This function takes a list of arguments and tries to set the parameters */
void cmdLineParse( int argc , char **argv, cmdLineReadable** params );

/** Returns the file extension */
char* GetFileExtension( const char* fileName );

/** Returns and array of individual words read from a file */
char** ReadWords( const char* fileName , int& cnt );

#include "cmdLineParser.inl"
#endif // CMD_LINE_PARSER_INCLUDED
