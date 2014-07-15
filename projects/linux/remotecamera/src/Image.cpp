/*
 * rc_image.cpp
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */
#include <stddef.h>
#include <setjmp.h>

#include "image.h"

bool Image::initialised = false;

jpeg_compress_struct *Image::jpg_ccinfo[101] = { 0 };
jpeg_decompress_struct *Image::jpg_dcinfo = 0;

Image::Image()
{
   // if ( !initialised )
   //     Initialise();

    width = 0;
    height = 0;
    pixels = 0;
    colours = 0;
    subpixelorder = 0;
    size = 0;
    allocation = 0;
    buffer = 0;
    buffertype = 0;
    holdbuffer = 0;
    text[0] = '\0';
}


Image::Image( const char *filename )
{
    //if ( !initialised )
    //    //Initialise();

    width = 0;
    height = 0;
    pixels = 0;
    colours = 0;
    subpixelorder = 0;
    size = 0;
    allocation = 0;
    buffer = 0;
    buffertype = 0;
    holdbuffer = 0;
    //ReadJpeg( filename, ZM_COLOUR_RGB24, ZM_SUBPIX_ORDER_RGB);
    text[0] = '\0';
}

Image::Image( int p_width, int p_height, int p_colours, int p_subpixelorder, uint8_t *p_buffer )
{
    //if ( !initialised )
    //    Initialise();
    width = p_width;
    height = p_height;
    pixels = width*height;
    colours = p_colours;
    subpixelorder = p_subpixelorder;
    size = (width*height)*colours;
    buffer = 0;
    holdbuffer = 0;

    text[0] = '\0';
}

Image::~Image(){

}

void Image::Initialise()
{

}


bool Image::ReadJpeg( const char *filename, unsigned int p_colours, unsigned int p_subpixelorder)
{

	printf("ReadJpeg \n");

	return true;
}
