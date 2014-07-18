package com.test.ubmedia.display;


import android.content.Context;
import android.view.SurfaceView;

public class VideoRender {
	 
	public static SurfaceView CreateRenderer(Context context) {
	
		return CreateRenderer(context, false);
	}

	public static SurfaceView CreateRenderer(Context context, boolean useOpenGLES2) {
		
		if(useOpenGLES2 == true && VideoOpenGLES20.IsSupported(context))
	    
			return new VideoOpenGLES20(context);
	    else
	        return new SurfaceView(context);
	}
}
