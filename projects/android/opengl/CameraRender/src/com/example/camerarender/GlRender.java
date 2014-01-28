package com.example.camerarender;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.Config;
import android.opengl.GLSurfaceView;

public class GlRender implements GLSurfaceView.Renderer {

	private int mWidth;
	private int mHeight;
	
	private Context mContext;
	private Bitmap mBitmap;
	private TextureVideo mTexture;
	
	public GlRender(Context context) {
		// TODO Auto-generated constructor stub
		
		mContext = context;
		
		mBitmap =  BitmapFactory.decodeResource(mContext.getResources(), R.drawable.img_3);
        mTexture = new TextureVideo();
	}
	
	public void update(Bitmap bitmap){
		
	}
	
	public void update(int[] rgb, int width, int height){
		
		mWidth = width;
		mHeight = height;
		mBitmap = Bitmap.createBitmap(rgb, mWidth, mHeight, Config.ARGB_8888);
		
	}

	
	@Override
	public void onDrawFrame(GL10 gl) {
		// TODO Auto-generated method stub
		
		gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
        gl.glLoadIdentity();
        gl.glTranslatef(0f, 0f, 0f);
        
        mTexture.bind(gl, mBitmap);
        mTexture.draw(gl, 0, 0);
        mTexture.delete(gl);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		// TODO Auto-generated method stub
		gl.glViewport(0, 0, width, height);
        gl.glMatrixMode(GL10.GL_PROJECTION);
        gl.glLoadIdentity();
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// TODO Auto-generated method stub
		
		gl.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		gl.glClearDepthf(1.0f);
		gl.glEnable(GL10.GL_DEPTH_TEST);
        gl.glDepthFunc(GL10.GL_LEQUAL);
        gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_NICEST);
        gl.glEnable(GL10.GL_CULL_FACE);
        gl.glShadeModel(GL10.GL_SMOOTH);
		
	}

}
