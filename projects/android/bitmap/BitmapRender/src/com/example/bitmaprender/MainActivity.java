package com.example.bitmaprender;

import java.nio.ByteBuffer;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class MainActivity extends Activity {
	
	private BitmapView  view = null;
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setContentView(R.layout.activity_main);
		
		LinearLayout tp = new LinearLayout(this);
		LayoutParams ltp = new LayoutParams(LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT);
		tp.setOrientation(1);
		
		
		//esay test case Button 
		Button bitmapFact = new Button(this);
		bitmapFact.setText("test BitmapFactory render");
		bitmapFact.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.d("", "test BitmapFactory render");
				
				Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.tulips);
				view.showBitmap(bitmap);
			}

		});
		
		//video decode test case Button 
		Button bitmap = new Button(this);
		bitmap.setText("test bitmap pixels render");
		bitmap.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Log.d("", "test bitmap pixels render");
				Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.queu);
			
				int width = bitmap.getWidth();
				int height = bitmap.getHeight();
				
				Log.e("bitmap render", "bitmap size is " + width * height);
				
				byte rgb[] = getRGBFromBitmap(bitmap);
				
				// convert rgb data to RGB888 and render
				int rgb8888[] = colorsToRGB8888(rgb);
				Bitmap bitmap2 = Bitmap.createBitmap(rgb8888, width, height,Bitmap.Config.ARGB_8888);
				//view.showBitmap(bitmap2);
				
				byte bRgb565[] = colorsToRGB565(rgb);
				ByteBuffer buffer = ByteBuffer.wrap(bRgb565);
				
				view.setRGB565Buffer(width, height, buffer);
				
			}

		});
		
		//video display surface 
		view = new BitmapView(this);

		tp.addView(bitmapFact, ltp);
		tp.addView(bitmap, ltp);
		tp.addView(view,ltp);
		
		setContentView(tp);	
	}
	
	private byte[] colorsToRGB565(byte[] rgb){
		
		//#define RGB565_MASK_RED        0xF800  
		//#define RGB565_MASK_GREEN      0x07E0  
		//#define RGB565_MASK_BLUE       0x001F  
		
		int size = rgb.length;
		
		short[] rgb565 = new short[size/3];
		
		// rgb color to rgb565
		// byte 类型转换为short类型
		for(int i = 0; i < rgb565.length; i++){
			
			rgb565[i] = (short)( rgb[i * 3] << 8 & 0xF800 |
					rgb[i * 3 + 1] << 3 & 0x07E0 |
					rgb[i * 3 + 2] >> 3);
		}
		
		Log.e("bitmap render", "rgb565[0] is "  + rgb565[0]);
		
		byte[] bRGB565 = new byte[rgb565.length * 2];
		//rgb565 convert to pixels
		// short 类型转换为byte类型
		for(int i = 0; i < rgb565.length; i++ ){
			
			bRGB565[i * 2] = (byte)(rgb565[i] >> 8 & 0xFF);
			bRGB565[i * 2+ 1] = (byte)(rgb565[i] & 0xFF); 
		}
		
		Log.e("bitmap render", "bRGB565[0] is "  + bRGB565[0]);
		
		return bRGB565;
	}
	
	private int[] colorsToRGB8888(byte[] rgb){
		
		int size = rgb.length;
		
		int[] rgb8888 = new int[size/3];
		
		for(int i = 0; i < rgb8888.length; i++){
			
			rgb8888[i] = (int)( (rgb[i * 3] << 16 & 0xFF0000) |
					(rgb[i * 3 + 1] << 8 & 0x00FF00) |
					(rgb[i * 3 + 2] & 0x0000FF) |
					0xFF000000 );
		}
		
		Log.e("bitmap render", "rgb8888[0] is "  + rgb8888[0]);
		
		return rgb8888;
	}
	

	
	private byte[] getRGBFromBitmap(Bitmap bitmap){
		
		if(bitmap == null) return null;
		
		
		int width = bitmap.getWidth();
		int height = bitmap.getHeight();
		int size = width * height;
		
		int[] pixels = new int[size];
		
		bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
		
		byte[] colors = new byte[pixels.length * 3];
		
		for(int i = 0; i < pixels.length; i++){
			
			// Alpha will not deal
			// pixels[i] >> 24;
			
			// get R color
			colors[i * 3] = (byte)(pixels[i] >> 16 & 0xFF);
			
			// get G color
			colors[i * 3 + 1] = (byte)(pixels[i] >> 8 & 0xFF);
			
			// get B color
			colors[i * 3 +2 ] = (byte)(pixels[i] & 0xFF);
		}
		
		Log.e("bitmap render", "pixels[0] is " + pixels[0] + " colors[0] is " + byte2HexStr(colors[0]));
		
		return colors;
	}
	
	
	
	public  String byte2HexStr(byte colors)  
    {  
        String stmp="";  
        StringBuilder sb = new StringBuilder("");  
        for (int n=0;n<1;n++)  
        {  
            stmp = Integer.toHexString(colors & 0xFF);  
            sb.append((stmp.length()==1)? "0"+stmp : stmp);  
            sb.append(" ");  
        }  
        return sb.toString().toUpperCase().trim();  
    } 

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
