/*
 * camera.cpp
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#include "camera.h"

Camera::Camera( int p_id, SourceType p_type, int p_width, int p_height, int p_colours, int p_subpixelorder, int p_brightness, int p_contrast, int p_hue, int p_colour, bool p_capture ) :
    id( p_id ),
    type( p_type ),
    width( p_width),
    height( p_height ),
    colours( p_colours ),
    subpixelorder( p_subpixelorder ),
    brightness( p_brightness ),
    hue( p_hue ),
    colour( p_colour ),
    contrast( p_contrast ),
    capture( p_capture )
{
	pixels = width * height;
	imagesize = pixels * colours;

	printf("New camera id: %d width: %d height: %d colours: %d subpixelorder: %d capture: %d",id,width,height,colours,subpixelorder,capture);

	/* Because many loops are unrolled and work on 16 colours/time or 4 pixels/time, we have to meet requirements */
	if( (imagesize % 16) != 0) {
		printf("Image size is not multiples of 16");
	} else if( ((imagesize % 16) != 0 || (imagesize % 12) != 0)) {
		printf("Image size is not multiples of 12 and 16");
	}
}

Camera::~Camera()
{
}


