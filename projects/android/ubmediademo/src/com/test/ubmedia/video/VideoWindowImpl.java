package com.test.ubmedia.video;

import com.test.ubmedia.activity.MainActivity;

import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class VideoWindowImpl {

	private boolean useGLrendering = false;
	private SurfaceView displaySurfaceView = null;
	private SurfaceView captureSurfaceView = null;
	
	
	
	public VideoWindowImpl(SurfaceView display, SurfaceView capture){
		
		
		this.displaySurfaceView = display;
		this.captureSurfaceView = capture;
		
		useGLrendering = (display instanceof GLSurfaceView);
	}
	
	
	public void init(){
		
		if(captureSurfaceView != null){
			
			// prepare the capture surface
		}
		
		displaySurfaceView.getHolder().addCallback(new Callback(){

			@Override
			public void surfaceCreated(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void surfaceChanged(SurfaceHolder holder, int format,
					int width, int height) {
				// TODO Auto-generated method stub
				
				//MainActivity.mediaEngine.um_vdec_setSurface();
			}

			@Override
			public void surfaceDestroyed(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				
			}
			
			
		});
		
		
		if (useGLrendering) {
			//renderer = new Renderer();
			//((GLSurfaceView)mVideoRenderingView).setRenderer(renderer);
			//((GLSurfaceView)mVideoRenderingView).setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		}
	}
}
