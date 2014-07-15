/*
 * rc_camera.h
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#ifndef RC_CAMERA_H_
#define RC_CAMERA_H_

#include "image.h"

#define PATH_MAX 256

/* A macro to use default subpixel order for a specified colour. */
/* for grayscale it will use NONE, for 3 colours it will use R,G,B, for 4 colours it will use R,G,B,A */
#define SUBPIX_ORDER_DEFAULT_FOR_COLOUR(c)	((c)<<1)


//
// Abstract base class for cameras. This is intended just to express
// common attributes
//
class Camera
{
protected:
	typedef enum { LOCAL_SRC, REMOTE_SRC, FILE_SRC } SourceType;

    int             id;
	SourceType		type;
	unsigned int	width;
	unsigned int	height;
	unsigned int	colours;
	unsigned int	subpixelorder;
	unsigned int	pixels;
	unsigned int	imagesize;
	int				brightness;
	int				hue;
	int				colour;
	int				contrast;
    bool            capture;

public:
	Camera( int p_id, SourceType p_type, int p_width, int p_height, int p_colours, int p_subpixelorder, int p_brightness, int p_contrast, int p_hue, int p_colour, bool p_capture );
	virtual ~Camera();

    int getId() const { return( id ); }
	SourceType Type() const { return( type ); }
	bool IsLocal() const { return( type == LOCAL_SRC ); }
	bool IsRemote() const { return( type == REMOTE_SRC ); }
	bool IsFile() const { return( type == FILE_SRC ); }
	unsigned int Width() const { return( width ); }
	unsigned int Height() const { return( height ); }
	unsigned int Colours() const { return( colours ); }
	unsigned int SubpixelOrder() const { return( subpixelorder ); }
	unsigned int Pixels() const { return( pixels ); }
	unsigned int ImageSize() const { return( imagesize ); }

	virtual int Brightness( int/*p_brightness*/=-1 ) { return( -1 ); }
	virtual int Hue( int/*p_hue*/=-1 ) { return( -1 ); }
	virtual int Colour( int/*p_colour*/=-1 ) { return( -1 ); }
	virtual int Contrast( int/*p_contrast*/=-1 ) { return( -1 ); }

    bool CanCapture() const { return( capture ); }

	virtual int PrimeCapture() { return( 0 ); }
	virtual int PreCapture()=0;
	virtual int Capture( Image &image )=0;
	virtual int PostCapture()=0;
};



#endif /* RC_CAMERA_H_ */
