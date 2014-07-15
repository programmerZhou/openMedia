/*
 * RemoteCamera.h
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */

#ifndef REMOTECAMERA_H_
#define REMOTECAMERA_H_

//
// Class representing 'remote' cameras, i.e. those which are
// accessed over a network connection.
//
class RemoteCamera : public Camera
{
protected:
	std::string	protocol;
	std::string	host;
	std::string	port;
	std::string	path;
	std::string	auth;
	std::string	auth64;

protected:
	struct addrinfo *hp;

public:
	RemoteCamera( int p_id,
			const std::string &p_proto,
			const std::string &p_host,
			const std::string &p_port,
			const std::string &p_path,
			int p_width,
			int p_height,
			int p_colours,
			int p_brightness,
			int p_contrast,
			int p_hue,
			int p_colour,
			bool p_capture );

	virtual ~RemoteCamera();

	const std::string &Protocol() const { return( protocol ); }
	const std::string &Host() const { return( host ); }
	const std::string &Port() const { return( port ); }
	const std::string &Path() const { return( path ); }
	const std::string &Auth() const { return( auth ); }

	virtual void Initialise();
	virtual void Terminate() = 0;
	virtual int Connect() = 0;
	virtual int Disconnect() = 0;
	virtual int PreCapture() = 0;
	virtual int Capture( Image &image ) = 0;
	virtual int PostCapture() = 0;
};


#endif /* REMOTECAMERA_H_ */
