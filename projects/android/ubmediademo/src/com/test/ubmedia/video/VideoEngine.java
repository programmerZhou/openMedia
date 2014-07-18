package com.test.ubmedia.video;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.media.MediaCodec;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceView;

import com.test.ubmedia.audio.AudioEngine;
import com.test.ubmedia.display.VideoRender;
import com.test.ubmedia.video.MediaCodecJavaImpl.MediaCodecListener;

public class VideoEngine {

private boolean vieRunning;
	
	private final int openglRender = 0;
	private final int surfaceViewRender = 1;
	private final int bitmapRender = 2;
	
	private SurfaceView svCapture = null;
	private SurfaceView svDisplay = null;
	
	
	private int mWidth;
	private int mHeight;
	private int codecType;
	private InputStream videoStream = null;
	
	// openGl, surfaceView
	private int viewSelection = 1;
	private final Context context;
	private VideoWindowImpl mVideoWindow = null;
	private MediaCodecJavaImpl mMediaCodecJavaImpl = null;
	
	public VideoEngine(Context context){
		 
		 this.context = context;
		 
	}
	 
	public void startVideoEngine(InputStream input, int width, int height){
		 
//		if (viewSelection == openglRender ){
//			
//			svDisplay = VideoRender.CreateRenderer(context, true);
//		} else if (viewSelection == surfaceViewRender ){
//	    
//			svDisplay = VideoRender.CreateRenderer(context, false);
//		} 
//		
//		mVideoWindow = new VideoWindowImpl(svDisplay, svCapture);
//		
//		mVideoWindow.init();
//		
//		
//		vieRunning = true;
		
		this.videoStream = input;
		
		startMediaCodecJavaImpl(videoStream, width, height);
	}
	
	private void startMediaCodecJavaImpl(InputStream input, int width, int height){
		
		svDisplay = VideoRender.CreateRenderer(context, false);
		
		String type = "video/avc";
		
		mMediaCodecJavaImpl = new MediaCodecJavaImpl(input, type, width, height);
		
		mMediaCodecJavaImpl.setVideoSurface(svDisplay);
		
		mMediaCodecJavaImpl.setVideoSurfaceListener(new MediaCodecListener(){

			@Override
			public void onVideoSurfaceReady(MediaCodecJavaImpl mc,
					SurfaceView surface) {
				// TODO Auto-generated method stub
				
				mMediaCodecJavaImpl.start();
			}

			@Override
			public void onVideoSurfaceDestroy(MediaCodecJavaImpl mc,
					SurfaceView surface) {
				// TODO Auto-generated method stub
				
			}
			
			
		});
		
		
	}
	
	
	public void stopVideoEngine(){
		
		if (!vieRunning) {
		      return;
		}
		
		
	}
	
	public void setViewSelection(int viewSelection) {
	
		this.viewSelection = viewSelection;
	}

	public int viewSelection() { 
		
		return viewSelection; 
	}
	
	public SurfaceView getCaptureSurfaceView() {
	    
		return svCapture;
	}

	public SurfaceView getDisplaySurfaceView() {
	    
		return svDisplay;
	}
	
	
	
}
