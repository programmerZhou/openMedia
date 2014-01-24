package com.example.bitmaprender;

import java.nio.ByteBuffer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.view.View;

public class BitmapView extends View{
	
	private Bitmap mBitmap = null;
	private Bitmap mRGB565Bitmap = null;
	private ByteBuffer mRGB565Buffer = null;
	
	public BitmapView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	public void showBitmap(Bitmap bitmap){
		
		mBitmap = bitmap;
		invalidate();
	}
	
	public void setRGB565Bitmap(short[] rgb565, int width, int height){
		
		
		//mRGB565Bitmap.
	}
	
	
	public void setRGB565Buffer(int width, int height, ByteBuffer buffer){
		
		mRGB565Bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
		
		mRGB565Buffer = buffer;
		
		mRGB565Bitmap.copyPixelsFromBuffer(buffer);  
		
		invalidate();
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		
		if(mBitmap != null){
		
			canvas.drawBitmap(mBitmap, 0, 0, null);
			//mBitmap = null;
		}
		
		if(mRGB565Bitmap != null){
			canvas.drawBitmap(mRGB565Bitmap, 0, 0, null);
		}
		
		
	}


}
