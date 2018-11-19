#include <string.h>
#include <stdlib.h>
#include "rayWindow.h"

const char RayWindow::KEY_ESCAPE = '\033';
RayScene*  RayWindow::scene = NULL;
Mouse      RayWindow::mouse;
int        RayWindow::curveFit = INTERPOLATION_LINEAR;
bool       RayWindow::isVisible;
int        RayWindow::frameCount;
Timer      RayWindow::timer;
double     RayWindow::radius;
double     RayWindow::frameRate;
Timer      RayWindow::frameCountTimer;
Point3D    RayWindow::center;
bool       RayWindow::_openGLSet = false;
int        RayWindow::_sceneComplexity;


/** This function prints out the state of the OpenGL error. */
int RayWindow::PrintError( int showNoError )
{
	int x , y;
	int e=1;
	switch( glGetError() )
	{
	case GL_NO_ERROR:          e=0                             ; break;
	case GL_INVALID_ENUM:      printf( "Invalid Enum\n" )      ; break;
	case GL_INVALID_VALUE:     printf( "Invalid Value\n" )     ; break;
	case GL_INVALID_OPERATION: printf( "Invalid Operation\n" ) ; break;
	case GL_STACK_OVERFLOW:    printf( "Stack Overflow\n" )    ; break;
	case GL_STACK_UNDERFLOW:   printf( "Stack Underflow\n" )   ; break;
	case GL_OUT_OF_MEMORY:     printf( "Out of memory\n" )     ; break;
	}
	if( !e && showNoError ) printf( "No error\n" );
	if( !showNoError && !e ) return 0;

	glGetIntegerv( GL_MATRIX_MODE , &x );
	if( x==GL_MODELVIEW )
	{
		glGetIntegerv( GL_MODELVIEW_STACK_DEPTH , &x );
		glGetIntegerv( GL_MAX_MODELVIEW_STACK_DEPTH , &y );
		printf( "Modelview Matrix: %d / %d\n" , x , y );
	}
	else if( x==GL_PROJECTION )
	{
		glGetIntegerv( GL_PROJECTION_STACK_DEPTH , &x );
		glGetIntegerv( GL_MAX_PROJECTION_STACK_DEPTH , &y );
		printf( "Projection Matrix: %d / %d\n" , x , y );
	}
	return e;
}

/** This function writes out the specified string, left-aligned, at the specified location, onto the OpenGL window. */
void RayWindow::WriteLeftString( int x , int y , const char* str )
{
	GLint vp[4];
	GLint dt = glIsEnabled( GL_DEPTH_TEST );
	GLint lm = glIsEnabled( GL_LIGHTING );
	GLint mm;

	glGetIntegerv( GL_VIEWPORT , vp );
	glGetIntegerv( GL_MATRIX_MODE , &mm );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0 , vp[2] , 0 , vp[3] , 0 , 1 );

	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glColor4f( 0 , 0 , 0 , 1 );
	glRasterPos2f( x , y );
	int len=(int)strlen(str);
	for( int i=0 ; i<len ; i++ ) glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18 , str[i] );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	if( dt ) glEnable( GL_DEPTH_TEST );
	if( lm ) glEnable( GL_LIGHTING );
	glMatrixMode( mm );
}
/** This function writes out the specified string, right-aligned, at the specified location, onto the OpenGL window. */
void RayWindow::WriteRightString (int x , int y , const char* str )
{
	GLint vp[4];
	glGetIntegerv( GL_VIEWPORT , vp );

	WriteLeftString( vp[2]-x-glutBitmapLength( GLUT_BITMAP_HELVETICA_18 , (unsigned char*)str ) , y , str );
}

/** This function reads the current frame buffer and sets the pixels of the image accordingly. */
Image32 RayWindow::TakeSnapshot( void )
{
	Image32 img;

	GLint vp[4];
	glGetIntegerv( GL_VIEWPORT , vp );

	GLfloat* pixels = new GLfloat[vp[2]*vp[3]*3];
	if( !pixels ) return img;
	img.setSize( vp[2] , vp[3] );
	glReadBuffer( GL_FRONT );
	glReadPixels( vp[0] , vp[1] , vp[2] , vp[3] , GL_RGB , GL_FLOAT , pixels );

	for( int i=0 ; i<vp[3] ; i++ ) for( int j=0 ; j<vp[2] ; j++ )
	{
		int idx;
		Pixel p;
		idx = 0+j*3+(vp[3]-i-1)*(vp[2])*3 ; p.r = 255*pixels[idx];
		idx = 1+j*3+(vp[3]-i-1)*(vp[2])*3 ; p.g = 255*pixels[idx];
		idx = 2+j*3+(vp[3]-i-1)*(vp[2])*3 ; p.b = 255*pixels[idx];
		img(j,i) = p;
	}
	delete[] pixels;
	return img;
}

/** This function is called when no events need to be processed. */
void RayWindow::IdleFunction( void )
{
	// Update the parameter values
	scene->setCurrentTime( timer.elapsed() , curveFit );
	// Just draw the scene again
	if( isVisible ) glutPostRedisplay();
}
/** This function is called when the visibility state of the window changes. */
void RayWindow::VisibilityFunction( int state )
{
	if     ( state==GLUT_VISIBLE     ) isVisible = true;
	else if( state==GLUT_NOT_VISIBLE ) isVisible = false;
}

//////////////////////////
// Mouse event handlers //
//////////////////////////
/** This function is called when the state of a mouse button is changed */
void RayWindow::MouseFunction( int button, int state, int x, int y ){ mouse.update( button , state , x , y ); }
/** This function is called when one of the mouse buttons is depressed and the mouse is moved. */
void RayWindow::MotionFunction( int x , int y )
{
	Point2D d = mouse.move(x,y);

	if     ( mouse.middleDown || (mouse.leftDown && mouse. ctrlDown ) ) scene->camera->rotateUp( center , 0.01*d[0] ) , scene->camera->rotateRight( center , 0.01*d[1] );
	else if( mouse.scrollDown || (mouse.leftDown && mouse.shiftDown ) ) scene->camera->moveForward( radius/15*d[1] );
	else if( mouse.leftDown ) scene->camera->moveRight( -radius/250*d[0] ) , scene->camera->moveUp( radius/250*d[1] );

	glutPostRedisplay();
}
/** This function is called when the mouse is moved moved but no buttons are depressed. */
void RayWindow::PassiveMotionFunction( int x , int y )
{
	mouse.move(x,y);
	glutPostRedisplay();
}

/////////////////////////////
// Keyboard event handlers //
/////////////////////////////
/** This function is called when the user presses a key. */
void RayWindow::KeyboardFunction( unsigned char c , int x , int y )
{
	char temp[500] , *t;
	Image32 img;

	switch( c )
	{
		case KEY_ESCAPE:
			exit( 0 );
			break;		
		case 'I':
			printf( "Image Name: " );
			t = fgets( temp , 500 , stdin );
			img = TakeSnapshot();
			img.write( temp );
			printf( "Wrote to file: %s\n" , temp );
			break;
		case 'p':
			fprintf( stderr,"\nPos: ( %g , %g , %g )\n Dir: ( %g , %g , %g )\n Up: ( %g , %g , %g )\n",
				scene->camera->position[0] , scene->camera->position[1] , scene->camera->position[2] ,
				scene->camera->direction[0] , scene->camera->direction[1] , scene->camera->direction[2] ,
				scene->camera->up[0] , scene->camera->up[1] , scene->camera->up[2] );
			break;
	}
}
/** This function is called when the user presses one of the special keys. */
void RayWindow::SpecialFunction( int key , int x , int y )
{
	switch( key )
	{
	case GLUT_KEY_F1:        break;
	case GLUT_KEY_F2:        break;
	case GLUT_KEY_F3:        break;
	case GLUT_KEY_F4:        break;
	case GLUT_KEY_F5:        break;
	case GLUT_KEY_F6:        break;
	case GLUT_KEY_F7:        break;
	case GLUT_KEY_F8:        break;
	case GLUT_KEY_F9:        break;
	case GLUT_KEY_F10:       break;
	case GLUT_KEY_F11:       break;
	case GLUT_KEY_F12:       break;
	case GLUT_KEY_UP:        break;
	case GLUT_KEY_DOWN:      break;
	case GLUT_KEY_LEFT:      break;
	case GLUT_KEY_RIGHT:     break;
	case GLUT_KEY_PAGE_UP:   break;
	case GLUT_KEY_PAGE_DOWN: break;
	case GLUT_KEY_HOME:      break;
	case GLUT_KEY_END:       break;
	case GLUT_KEY_INSERT:    break;
	}
	glutPostRedisplay();
}

/////////////////////////
// Menu event handlers //
/////////////////////////
/** This function is called when the user updates the draw mode in the drop-down menu. */
void RayWindow::DrawModeMenu( int entry )
{
	glPolygonMode( GL_FRONT_AND_BACK , entry );
	glutPostRedisplay();
}
/** This function is called when the user updates the cull mode in the drop-down menu. */
void RayWindow::CullModeMenu( int entry )
{
	if( entry==NO_CULL ) glDisable( GL_CULL_FACE );
	else
	{
		glEnable( GL_CULL_FACE );
		if( entry==CULL_BACK_FACE  ) glCullFace( GL_BACK  );
		if( entry==CULL_FRONT_FACE ) glCullFace( GL_FRONT );
	}
	glutPostRedisplay();
}
/** This function is called when the user updates the curve fitting method in the drop-down menu. */
void RayWindow::CurveFitMenu( int entry )
{
	curveFit = entry;
	timer = Timer();
	glutPostRedisplay();
}
/** This function is called when the user selects one of the main menu options in the drop-down menu. */
void RayWindow::MainMenu( int entry )
{
	if( !entry ) exit( 0 );
}

/**  This function draws the OpenGL window. */
void RayWindow::DisplayFunction( void )
{
	if( !_openGLSet ) scene->setUpOpenGL( _sceneComplexity ) , _openGLSet = true;

	char temp[500];

	// Clear the color and depth buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	float d = radius + ( scene->camera->position - center ).length();

	// Draw the perspective projection (to get good front and back clipping planes, we need to know the rough
	// size of the model)
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( scene->camera->heightAngle * 180./PI , scene->camera->aspectRatio , .1*d , 2*d );
	if( scene->shader && scene->shader->glslProgram )
	{
		static bool firstTime = true;
		GLdouble projection[16];
		glGetDoublev( GL_PROJECTION_MATRIX , projection );
		scene->shader->glslProgram->setUniformMatrix< 4 >( "projection" , projection , firstTime );
		firstTime = false;
	}

	// Draw the RayScene
	GLint drawMode[2];
	glGetIntegerv( GL_POLYGON_MODE , drawMode );
	if( drawMode[0]==GL_FILL ) scene->drawOpenGL();
	else
	{
		glColorMask( GL_FALSE , GL_FALSE , GL_FALSE , GL_FALSE );
		glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
		glPolygonOffset( 1 , 2);
		glDisable( GL_BLEND );
		scene->drawOpenGL();
		glDisable( GL_POLYGON_OFFSET_FILL );
		glColorMask( GL_TRUE , GL_TRUE , GL_TRUE , GL_TRUE );
		glPolygonMode( GL_FRONT_AND_BACK , drawMode[0] );
		glDisable( GL_BLEND );
		glEnable( GL_LINE_SMOOTH );
		glLineWidth(2);
		scene->drawOpenGL();
	}

	glUseProgram( 0 );

	// Update the frame rate value on the tenth frame
	frameCount++;
	if( frameCount==10 )
	{
		frameCount = 0;
		frameRate = 10. / frameCountTimer.elapsed();
		frameCountTimer = Timer();
	}
	sprintf( temp , "%.1f fs" , frameRate );
	WriteLeftString( 1 , 2 , temp );

	// Write out the mouse position
	sprintf( temp , "( %3d , %3d )" , mouse.endX , mouse.endY );
	WriteRightString( 1 , 2 , temp );

	if( scene->shader && scene->shader->glslProgram ) scene->shader->glslProgram->use();

	// Swap the back and front buffers
	glutSwapBuffers();
}

/** This function is called when the window is resized. */
void RayWindow::ReshapeFunction( int width , int height )
{
	GLint viewPort[4];
	
	glViewport( 0 , 0 , width , height );
	glGetIntegerv( GL_VIEWPORT , viewPort );
	scene->camera->aspectRatio = (float)width/(float)height;
	glutPostRedisplay();
}

/** This function instantiates the OpenGL window, reading in the RayScene from the specified file
  * and setting the initial OpenGL window size. The function never returns! */
void RayWindow::RayView( RayScene& s , int width , int height , int cplx )
{
	_sceneComplexity = cplx;
	int drawMenu;
	int cullMenu;
	int curveFitMenu;
	
	timer = Timer();
	scene = &s;
	BoundingBox3D b = scene->group->setBoundingBox();
	center = ( b.p[0] + b.p[1] ) / 2;
	radius = ( b.p[0] - b.p[1] ).length() / 2;
	frameCountTimer = Timer();

	// Initialize the OpenGL context
	int argc = 1;
	char* argv = new char[ strlen("hello")+1 ];
	strcpy( argv , "hello" );
	glutInit( &argc , &argv );
	delete[] argv;

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow( "OpenGL Visualization" );
	if( glewInit()!=GLEW_OK ) fprintf( stderr , "[ERROR] glewInit failed\n" ) , exit(0);
	glutInitWindowSize( width , height );
	glutInitWindowPosition( 0 , 0 );
	glClearColor( scene->background[0] , scene->background[1] , scene->background[2] , 1. );
	glutReshapeWindow( width , height );

	// Initialize the event handlers
	glutDisplayFunc      ( DisplayFunction );
	glutReshapeFunc      ( ReshapeFunction );
	glutKeyboardFunc     ( KeyboardFunction );
	glutSpecialFunc      ( SpecialFunction );
	glutMouseFunc        ( MouseFunction );
	glutMotionFunc       ( MotionFunction );
	glutPassiveMotionFunc( PassiveMotionFunction );
	glutVisibilityFunc   ( VisibilityFunction );
	glutIdleFunc         ( IdleFunction );

	// Initialize the menus
	drawMenu = glutCreateMenu( DrawModeMenu );
	glutAddMenuEntry( " Polygons " , GL_FILL );
	glutAddMenuEntry( " Lines " , GL_LINE );
	glutAddMenuEntry( " Points " , GL_POINT );

	cullMenu = glutCreateMenu( CullModeMenu );
	glutAddMenuEntry( " None " , NO_CULL );
	glutAddMenuEntry( " Back " , CULL_BACK_FACE );
	glutAddMenuEntry( " Front " , CULL_FRONT_FACE );

	curveFitMenu = glutCreateMenu( CurveFitMenu );
	for( int i=0 ; i<INTERPOLATION_COUNT ; i++ ) glutAddMenuEntry( InterpolationNames[i] , i );

	glutCreateMenu( MainMenu );
	glutAddSubMenu( " Polygon mode " , drawMenu );
	glutAddSubMenu( " Cull mode " , cullMenu );
	glutAddSubMenu( " Animation Fitting " , curveFitMenu );
	glutAddMenuEntry( " Quit " , 0 );

	// Attach the drop-down menu to the right button
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_NORMALIZE );
	glDepthMask( GL_TRUE );
	glDisable( GL_BLEND );
	glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

	// Fall into the main loop
	glutMainLoop();
}
