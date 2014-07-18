package com.test.ubmedia.video;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.LinkedList;

import android.media.MediaCodec;
import android.media.MediaCodec.BufferInfo;
import android.media.MediaCodecInfo;
import android.media.MediaCrypto;
import android.media.MediaFormat;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class MediaCodecJavaImpl {

	private final String TAG = "MediaCodecJavaImpl";
	private String mMime;
	private int mWidth;
	private int mHeight;
	private MediaCodec codec = null;
	private Surface mSurface;
	private SurfaceView mSurfaceView = null;
	private InputStream videoStream = null;
	private MediaCodecListener listener = null;
	private PlayerThread mPlayerThread = null;

	// Offset between system time and media time.
	private long deltaTimeUs;
	
	public static interface MediaCodecListener{
		
		public void onVideoSurfaceReady(MediaCodecJavaImpl mc, SurfaceView surface);
		public void onVideoSurfaceDestroy(MediaCodecJavaImpl mc, SurfaceView surface);
	}

	
	public MediaCodecJavaImpl(InputStream input, String mime, int width, int height){
		
		this.videoStream = input;
		this.mMime = mime;
		this.mWidth = width;
		this.mHeight = height;

	}
	
	public void setVideoSurfaceListener(MediaCodecListener listener){
		
		this.listener = listener;
	}
	
	public void setVideoSurface(SurfaceView surfaceView){
		
		mSurfaceView = surfaceView;
		
		surfaceView.getHolder().addCallback(new Callback(){

			@Override
			public void surfaceCreated(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void surfaceChanged(SurfaceHolder holder, int format,
					int width, int height) {
				// TODO Auto-generated method stub
				
				mSurface = holder.getSurface();
				listener.onVideoSurfaceReady(MediaCodecJavaImpl.this, mSurfaceView);
			}

			@Override
			public void surfaceDestroyed(SurfaceHolder holder) {
				// TODO Auto-generated method stub
				
				listener.onVideoSurfaceDestroy(MediaCodecJavaImpl.this, mSurfaceView);
			}
			
			
		});
	}
	
	
	
	public boolean start(){
		
		mPlayerThread = new PlayerThread( );
		mPlayerThread.start();
		
		return true;
	}
	
	public void stop(){
		
		if(mPlayerThread != null){
			
			mPlayerThread.stopThread();
		}
	}
	
	
	private int byteToInteger(byte[] buf) {
		int iCount = 0;

		for (int i = 0; i < 4; i++) {

			int n = (buf[i] < 0 ? (int) buf[i] + 256 : (int) buf[i] << (8 * i));
			iCount += n;
		}

		return iCount;
	}
	
	
	class PlayerThread extends Thread{
		
		private boolean run = true;
		private MediaCodec codec = null;
		
		
		PlayerThread(){
			
			MediaFormat format = MediaFormat.createVideoFormat(mMime, mWidth, mHeight);
			format.setInteger(MediaFormat.KEY_BIT_RATE, 100000);
			format.setInteger(MediaFormat.KEY_FRAME_RATE, 15);
			
			format.setInteger(MediaFormat.KEY_COLOR_FORMAT,
						MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar);
					
			String mime = format.getString(MediaFormat.KEY_MIME);
			format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 5);  
			
			codec = MediaCodec.createDecoderByType(mime);
			
			Log.e(TAG, " create decode by type mime " + mime);
			
			if(codec == null){
				
				Log.e(TAG, " create decode by type fail");
				return;
			}
			
			codec.configure(format, mSurface, null, 0);
			codec.start();
		}
		
		public void stopThread(){
			
			run = false;
		}
		
		public void run(){
				
			ByteBuffer[] inputBuffers = codec.getInputBuffers();
			ByteBuffer[] outputBuffers = codec.getOutputBuffers();
			
			byte[] bLen = new byte[4];
			byte[] bFrame = new byte[40960];
			int frameLen = 0;
			
		
			Log.e(TAG,"run  thread");
			
			while (!Thread.currentThread().isInterrupted() && run) {
				Log.e(TAG,"run  thread 1 ");
				
				int bytes = 0;
				
				try {
					bytes = videoStream.read(bLen, 0, 4);
					Log.e(TAG,"run  thread 2 ");
				
					if(bytes == -1){
						
						Log.e(TAG, "release Thread.currentThread().interrupt()");
						Thread.currentThread().interrupt();
						run = false;
						break;
					}
					
					bytes = videoStream.read(bLen, 0, 4);
					frameLen = byteToInteger(bLen);
					
					bytes = videoStream.read(bFrame, 0, frameLen);
					Log.e(TAG,"bytesRead " + bytes);
					

					
					if(bytes > 0){
						
						int inputBufferIndex = codec.dequeueInputBuffer(0);
						
						if(inputBufferIndex >= 0){
							
							ByteBuffer inputeBuffer = inputBuffers[inputBufferIndex];
							inputeBuffer.clear();
							inputeBuffer.put(bFrame, 0, frameLen);
							
							codec.queueInputBuffer(inputBufferIndex, 0, frameLen, 0, 0);
						}
						
						BufferInfo info = new BufferInfo();
						
						int outputBufferIndex = codec.dequeueOutputBuffer(info, 0);
						
						if(outputBufferIndex >= 0){
							
							Log.e(TAG, "codec outputBufferIndex" + outputBufferIndex);
							codec.releaseOutputBuffer(outputBufferIndex, true);
							outputBufferIndex = codec.dequeueOutputBuffer(info, 0);
						}else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
							
							Log.e(TAG, "codec INFO_OUTPUT_BUFFERS_CHANGED");
							outputBuffers = codec.getOutputBuffers();
						} else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
						    
							Log.e(TAG, "codec INFO_OUTPUT_FORMAT_CHANGED");
							// Subsequent data will conform to new format.
						    MediaFormat format = codec.getOutputFormat();
						    
						 }
						
					}
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();	
				}
			}
			
			
			try {
				if (videoStream != null){
					
					videoStream.close();
				}
				
				Log.e(TAG, "release codec");
				codec.stop();
				codec.release();
				codec = null;
				
			} catch (IOException e) {
				e.printStackTrace();
			}
				
				
			}
		}
}
