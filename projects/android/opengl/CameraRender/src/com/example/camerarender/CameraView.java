package com.example.camerarender;

import java.io.IOException;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraView extends SurfaceView implements SurfaceHolder.Callback {

	private int mPreviewWidth = 800;
	private int mPreviewHeight = 480;
	
	private Camera mCamera = null;
	private SurfaceHolder mSurfaceHolder = null;
	
	public CameraView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		
		mSurfaceHolder = this.getHolder();
		mSurfaceHolder.addCallback(this);
		mSurfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int arg1, int arg2, int arg3) {
		// TODO Auto-generated method stub
		Camera.Parameters parameters = mCamera.getParameters();
		parameters.setPreviewSize(mPreviewWidth, mPreviewHeight);
		parameters.setPreviewFormat(PixelFormat.YCbCr_420_SP );
		
		if (this.getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE) {
			parameters.set("orientation", "portrait"); //
			//parameters.set("rotation", 90); // 镜头角度转90度（默认摄像头是横拍）
			mCamera.setDisplayOrientation(90); // 在2.2以上可以使用
		} else{ // 如果是横屏
			parameters.set("orientation", "landscape"); //
			mCamera.setDisplayOrientation(0); // 在2.2以上可以使用
		}

		
		//mCamera.setParameters(parameters);
		
		try {
			mCamera.setPreviewDisplay(holder);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		mCamera.startPreview();
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
		
		//mCamera = Camera.open();
		mCamera = Camera.open(0);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
	     
		if(null != mCamera)
        {
         
			mCamera.stopPreview(); 
			mCamera.release();
			mCamera = null;
        }
        
        mSurfaceHolder = null;
	}

}
