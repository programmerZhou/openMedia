/*
 * minitor.cpp
 *
 *  Created on: 2014-3-25
 *      Author: Administrator
 */


#include "Monitor.h"



Monitor::Monitor(
    int p_id,
    const char *p_name,
    int p_function,
    bool p_enabled,
    const char *p_linked_monitors,
    Camera *p_camera,
    int p_orientation,
    unsigned int p_deinterlacing,
    const char *p_event_prefix,
    const char *p_label_format,
    const Coord &p_label_coord,
    int p_image_buffer_count,
    int p_warmup_count,
    int p_pre_event_count,
    int p_post_event_count,
    int p_stream_replay_buffer,
    int p_alarm_frame_count,
    int p_section_length,
    int p_frame_skip,
    int p_capture_delay,
    int p_alarm_capture_delay,
    int p_fps_report_interval,
    int p_ref_blend_perc,
    int p_alarm_ref_blend_perc,
    bool p_track_motion,
//    Rgb p_signal_check_colour,
    Purpose p_purpose,
    int p_n_zones
//    Zone *p_zones[]
	) : id( p_id ),
    function( (Function)p_function ),
    enabled( p_enabled ),
    width( (p_orientation==ROTATE_90||p_orientation==ROTATE_270)?p_camera->Height():p_camera->Width() ),
    height( (p_orientation==ROTATE_90||p_orientation==ROTATE_270)?p_camera->Width():p_camera->Height() ),
    orientation( (Orientation)p_orientation ),
    deinterlacing( p_deinterlacing ),
//    label_coord( p_label_coord ),
    image_buffer_count( p_image_buffer_count ),
    warmup_count( p_warmup_count ),
    pre_event_count( p_pre_event_count ),
    post_event_count( p_post_event_count ),
    stream_replay_buffer( p_stream_replay_buffer ),
    section_length( p_section_length ),
    frame_skip( p_frame_skip ),
    capture_delay( p_capture_delay ),
    alarm_capture_delay( p_alarm_capture_delay ),
    alarm_frame_count( p_alarm_frame_count ),
    fps_report_interval( p_fps_report_interval ),
    ref_blend_perc( p_ref_blend_perc ),
    alarm_ref_blend_perc( p_alarm_ref_blend_perc ),
    track_motion( p_track_motion ),
//    signal_check_colour( p_signal_check_colour ),
//    delta_image( width, height, ZM_COLOUR_GRAY8, ZM_SUBPIX_ORDER_NONE ),
    ref_image( width, height, p_camera->Colours(), p_camera->SubpixelOrder() ),
    purpose( p_purpose )
//    camera( p_camera )
//    n_zones( p_n_zones )
//    zones( p_zones )
{

}


Monitor::Monitor(Camera* camera){

	this->camera = camera;
}


Monitor::~Monitor()
{

}


int Monitor::Capture(){

	Image image;

	camera->Capture(image);

	return 0;
}


int Monitor::LoadFileMonitors( const char *file, Monitor **&monitors, Purpose purpose )
{

	delete[] monitors;
	monitors = new Monitor *[1];


	int id = 0;
	int cam_width = 640;
	int cam_height = 540;
	int colours = 0;
	int brightness = 0;
	int contrast = 0;
	int hue = 0;
	int colour = 0;

	Camera *camera = new FileCamera(
		id,
		file, // File
		cam_width,
		cam_height,
		colours,
		brightness,
		contrast,
		hue,
		colour,
		purpose==CAPTURE
	);

	monitors[0] = new Monitor(camera);

}
