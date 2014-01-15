package com.example.openglrender;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.app.Activity;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class TestCaseActivity extends Activity implements Runnable, SurfaceHolder.Callback {

	
	private int mCodecType = 1; // 1 h264; 2 mpeg4-2
	private int mCodec = 1; // 0 sw_ffmpeg; 1 sw_opencore
	private static SurfaceView mSurfaceView;
	private SurfaceHolder holder;
	private final String LAG = "This is a test";

	int mTrans = 0x0F0F0F0F;
	String PathFileName;
	String fileName;

	// Menu item Ids
	public static final int PLAY_ID = Menu.FIRST;
	public static final int EXIT_ID = Menu.FIRST + 1;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_test_case);
		
		LinearLayout tp = new LinearLayout(this);
		LayoutParams ltp = new LayoutParams(LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT);
		tp.setOrientation(1);

		
		//circle lsit test Button 
		Button list = new Button(this);
		// list.setWidth(100);
		list.setText("test circle list");
		list.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Log.d("", "test circke list click");
				//test_circle_list();
			}

		});

		//esay test case Button 
		Button test = new Button(this);
		test.setText("test case");
		test.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.d("", "test case");
				//test();
			}

		});
		
		//video decode test case Button 
		Button decode = new Button(this);
		decode.setText("test video decode");
		decode.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.d("", "test test_video_dec()");
				if(mCodecType == 1){
					PathFileName = "/mnt/sdcard/predecode.h264"; // 352x288.264"; //240x320.264";
					fileName = "predecode.h264";
				}
				else if(mCodecType == 2){
					PathFileName = "/mnt/sdcard/predecode.m4v"; // 352x288.264"; //240x320.264";
					fileName = "predecode.m4v";
				}
				startDecode();
			}

		});
		
		//video display surface 
		mSurfaceView = new GameGlView(this);
		mSurfaceView.setVisibility(View.GONE);
		mSurfaceView.setVisibility(View.VISIBLE);
		holder = mSurfaceView.getHolder();
		holder.addCallback(this);
		holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);	

		tp.addView(decode, ltp);
		tp.addView(mSurfaceView,ltp);
		setContentView(tp);	
		
	}
	
	public void onStart(){
		
		Log.e("test", "onStart");
		if(mCodec == 0){	
		}
		um_vdec_init(mCodec, this.getPackageName(), Build.VERSION.SDK_INT);
		
		super.onStart();
	}
	
	public void onDestroy(){
		Log.e("test", "onDestroy");
		um_vrend_destroy();
		um_vdec_destroy();
		
		super.onDestroy();
	}

	//invoke the run()
	private void startDecode(){
		new Thread(this).start();
	}
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);

		menu.add(0, PLAY_ID, 0, "play");
		menu.add(0, EXIT_ID, 1, "exit");

		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case PLAY_ID: {

			return true;
		}
		case EXIT_ID: {
			Log.e("jack", "EXIT_ID");
			finish();
			return true;
		}
		}
		return super.onOptionsItemSelected(item);
	}

	public native int um_vdec_init(int codec, String pkg, int version);

	public native int um_vdec_create(int type, int width, int height);
	
	public native int um_vdec_decode(byte[] buf, int len);

	public native int um_vdec_destroy();
	
	public native int um_vdec_fini();
	
	public native int um_vdec_setSurface();
	
	public static native void um_vrend_renderFrame();
	
	public static native int um_vrend_create();
	
	public static native int um_vrend_destroy();
	
	public static native int um_vrend_resize(int width, int height);
	
	static {
		System.loadLibrary("testcast");
	}
	
	private boolean checkStartMar(byte[] buf) {

		if ((buf[0] == 0) && (buf[1] == 0) && (buf[2] == 0) && (buf[3] == 10)) {

			Log.d(LAG, "find the start mark");
			return true;
		}

		return false;
	}



	private int byteToInteger(byte[] buf) {
		int iCount = 0;

		for (int i = 0; i < 4; i++) {

			int n = (buf[i] < 0 ? (int) buf[i] + 256 : (int) buf[i] << (8 * i));
			iCount += n;
		}

		return iCount;
	}

	
	@Override
	public void run() {
		Log.d("test", "test thread start");

		//FileInputStream fileIS = null;
		InputStream fileIS = null;

		byte[] bLen = new byte[4];
		byte[] bFrame = new byte[40980];
		int frameLen;

		try {
			Log.d("jack", "FileInputStream PathFileName =" + PathFileName);
			//fileIS = new FileInputStream(PathFileName);
			
			fileIS = getResources().getAssets().open(fileName);
			
		} catch (IOException e) {
			Log.d("jack", "FileInputStream exception");
			return;
		}

		um_vdec_create(mCodecType, 800, 480);
		
		while (!Thread.currentThread().isInterrupted()) {

			try {

				int bytesRead = fileIS.read(bLen, 0, 4);
				Log.e(LAG,"bytesRead " + bytesRead);
				if(bytesRead == -1){
					Thread.currentThread().interrupt();
				}
						
				bytesRead = fileIS.read(bLen, 0, 4);
				Log.e(LAG,"bytesRead " + bytesRead);
				frameLen = byteToInteger(bLen);
				
				bytesRead = fileIS.read(bFrame, 0, frameLen);
				Log.e(LAG,"bytesRead " + bytesRead);
				
//				try {
//					Thread.sleep(10);
//				} catch (InterruptedException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}

				um_vdec_decode(bFrame, frameLen);

			} catch (IOException e) {
				Log.d(LAG, "read the video resource exception");
			}
			

		}
		try {
			if (fileIS != null){
				Log.d(LAG, "read the video resource end========");
				
				Log.d(LAG, "read the video resource um_vdec_destroy");
				fileIS.close();
				Log.d(LAG, "read the video resource fileIS.close");
			}
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		Log.d(LAG, "thread exit");
	}
	
	private void updateSurfaceScreenOn() {
		if (holder != null) {
			holder.setKeepScreenOn( true);
		}
	}
	

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO Auto-generated method stub
		Log.e("test", "==surfaceChanged");
		
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		//draw();
		Log.e("test", "==surfaceCreated");
		//this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		mSurfaceView.postInvalidate();
	
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		Log.e("test", "==surfaceDestroyed");
		
	}
	
	public static void notifyOpenGlDraw() {
		
		Log.d("test", "notifyOpenGlDraw");
		((GameGlView)mSurfaceView).requestRender();
		return ;
		
	}

}
