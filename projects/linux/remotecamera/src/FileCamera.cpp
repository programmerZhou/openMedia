/*
 * FileCamera.cpp
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "FileCamera.h"


FileCamera::FileCamera( int p_id, const char *p_path, int p_width, int p_height, int p_colours, int p_brightness, int p_contrast, int p_hue, int p_colour, bool p_capture )
	: Camera( p_id, FILE_SRC, p_width, p_height, p_colours, SUBPIX_ORDER_DEFAULT_FOR_COLOUR(p_colours), p_brightness, p_contrast, p_hue, p_colour, p_capture )
{
	strncpy( path, p_path, sizeof(path) );
	if ( capture )
	{
		Initialise();
	}
}

FileCamera::~FileCamera()
{
	if ( capture )
	{
		Terminate();
	}
}

void FileCamera::Initialise()
{
	if ( !path[0] )
	{
		printf( "No path specified for file image\n" );
		return;
	}
}

void FileCamera::Terminate()
{
}

int FileCamera::PreCapture()
{
	struct stat statbuf;
	if ( stat( path, &statbuf ) < 0 )
	{
		printf( "Can't stat %s\n", path );
		return( -1 );
	}

	while ( (time( 0 ) - statbuf.st_mtime) < 1 )
	{
		usleep( 100000 );
	}
	return( 0 );
}

int FileCamera::Capture( Image &image )
{
	return( image.ReadJpeg( path, colours, subpixelorder )?0:-1 );
}

int FileCamera::PostCapture()
{
    return( 0 );
}
