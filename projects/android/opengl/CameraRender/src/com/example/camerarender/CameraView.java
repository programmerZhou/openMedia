package com.example.camerarender;

import java.io.IOException;
import java.util.List;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.AttributeSet;
import android.util.Log;
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
		
		List<Integer> previewPixelFormat = parameters.getSupportedPreviewFormats();
		
		for(int i = 0; i < previewPixelFormat.size(); i++){
			Log.i("camera render", "getSupportedPreviewFormats" + previewPixelFormat.get(i));
		}
		
		//  ���������û��ʲô���壬��һЩ�豸�ϣ�������ú�Ӳ��ʵ�ʲ�����ƥ��Ļ���������crash
		//parameters.setPreviewSize(mPreviewWidth, mPreviewHeight);
		parameters.setPreviewFormat(PixelFormat.YCbCr_420_SP );
		
		if (this.getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE) {
			parameters.set("orientation", "portrait"); //
			//parameters.set("rotation", 90); // ��ͷ�Ƕ�ת90�ȣ�Ĭ������ͷ�Ǻ��ģ�
			mCamera.setDisplayOrientation(90); // ��2.2���Ͽ���ʹ��
		} else{ // ����Ǻ���
			parameters.set("orientation", "landscape"); //
			mCamera.setDisplayOrientation(0); // ��2.2���Ͽ���ʹ��
		}

		mCamera.setParameters(parameters);
		
		mCamera.setPreviewCallback(new PreviewCallback(){

			@Override
			public void onPreviewFrame(byte[] data, Camera camera) {
				// TODO Auto-generated method stub
				int width = camera.getParameters().getPreviewSize().width;
				int height = camera.getParameters().getPreviewSize().height;
				
				Log.e("camera callback","width is " + width + " height is " + height);
			
				int[] rgb = new int[width * height];
				
				Utils.covertYUV420SPToRGB565(rgb, data, width, height);
				
				MainActivity.getRender().update(rgb, width, height);
			}
			
		});
		
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
         
			mCamera.setPreviewCallback(null);
			mCamera.stopPreview(); 
			mCamera.release();
			mCamera = null;
        }
        
        mSurfaceHolder = null;
	}

}
