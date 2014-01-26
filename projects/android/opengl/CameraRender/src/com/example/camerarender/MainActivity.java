package com.example.camerarender;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.Window;
import android.widget.LinearLayout;

public class MainActivity extends Activity {

	static GlRender mGlRender;
	CameraView mCameraView;
	GLSurfaceView mGlRenderView;
	
	LinearLayout mLayout;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		//getWindow().requestFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_main);
		
		mCameraView = (CameraView)findViewById(R.id.cameraview);
		mCameraView.setContext(this);
		mLayout = (LinearLayout)findViewById(R.id.processview_layout);
		
		mGlRenderView = new GLSurfaceView(this);
		mLayout.addView(mGlRenderView);
		
		mGlRender = new GlRender(this);
		mGlRenderView.setRenderer(mGlRender);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	
	public  static GlRender getRender(){
		
		return mGlRender;
	}

}
