/*
 * image.h
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdio.h>
#include "jpeglib.h"
#include "jerror.h"
//
// This is image class, and represents a frame captured from a
// camera in raw form.
//
class Image
{
protected:

//	struct Edge
//	{
//		int min_y;
//		int max_y;
//		double min_x;
//		double _1_m;
//
//		static int CompareYX( const void *p1, const void *p2 )
//		{
//			const Edge *e1 = (const Edge *)p1, *e2 = (const Edge *)p2;
//			if ( e1->min_y == e2->min_y )
//				return( int(e1->min_x - e2->min_x) );
//			else
//				return( int(e1->min_y - e2->min_y) );
//		}
//		static int CompareX( const void *p1, const void *p2 )
//		{
//			const Edge *e1 = (const Edge *)p1, *e2 = (const Edge *)p2;
//			return( int(e1->min_x - e2->min_x) );
//		}
//	};
//
//	inline void DumpImgBuffer() {
//		DumpBuffer(buffer,buffertype);
//		buffer = NULL;
//		allocation = 0;
//	}
//
//	inline void AllocImgBuffer(size_t p_bufsize) {
//		if(buffer)
//			DumpImgBuffer();
//
//		buffer = AllocBuffer(p_bufsize);
//		buffertype = ZM_BUFTYPE_ZM;
//		allocation = p_bufsize;
//	}

public:
	enum { CHAR_HEIGHT=11, CHAR_WIDTH=6 };
	enum { LINE_HEIGHT=CHAR_HEIGHT+0 };

protected:
	static bool initialised;
	static unsigned char *abs_table;
	static unsigned char *y_r_table;
	static unsigned char *y_g_table;
	static unsigned char *y_b_table;
	static jpeg_compress_struct *jpg_ccinfo[101];
	static jpeg_decompress_struct *jpg_dcinfo;
	static struct zm_error_mgr jpg_err;

protected:
	unsigned int width;
	unsigned int height;
	unsigned int pixels;
	unsigned int colours;
	unsigned int size;
	unsigned int subpixelorder;
	unsigned long allocation;
	uint8_t *buffer;
	int buffertype; /* 0=not ours, no need to call free(), 1=malloc() buffer, 2=new buffer */
	int holdbuffer; /* Hold the buffer instead of replacing it with new one */
	char text[1024];

protected:
	static void Initialise();

public:
	Image();
	Image( const char *filename );
	Image( int p_width, int p_height, int p_colours, int p_subpixelorder, uint8_t *p_buffer=0);
	Image( const Image &p_image );
	~Image();

	inline unsigned int Width() const { return( width ); }
	inline unsigned int Height() const { return( height ); }
	inline unsigned int Pixels() const { return( pixels ); }
	inline unsigned int Colours() const { return( colours ); }
	inline unsigned int SubpixelOrder() const { return( subpixelorder ); }
	inline unsigned int Size() const { return( size ); }

	bool ReadJpeg( const char *filename, unsigned int p_colours, unsigned int p_subpixelorder);
};


#endif /* IMAGE_H_ */
