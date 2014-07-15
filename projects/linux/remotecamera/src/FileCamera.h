/*
 * FileCamera.h
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#ifndef FILECAMERA_H_
#define FILECAMERA_H_

#include "Camera.h"

class FileCamera : public Camera
{
protected:
	char path[PATH_MAX];

public:
	FileCamera( int p_id, const char *p_path,
			int p_width,
			int p_height,
			int p_colours,
			int p_brightness,
			int p_contrast,
			int p_hue,
			int p_colour,
			bool p_capture );

	~FileCamera();

	const char *Path() const { return( path ); }

	void Initialise();
	void Terminate();
	int PreCapture();
	int Capture( Image &image );
	int PostCapture();
};

#endif /* FILECAMERA_H_ */
