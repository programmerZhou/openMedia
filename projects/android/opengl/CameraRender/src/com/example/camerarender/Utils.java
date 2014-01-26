package com.example.camerarender;

public class Utils {
	
	private static int initialized = 0;
	private static int mCrTable[] = new int[256];
	private static int mCbTable[] = new int[256];
	private static int mYTable[] = new int[256];
	private static int mYCrTable[][] = new int[256][256];
	private static int mYCbTable[][] = new int[256][256];
	
	
	public static void covertYUV420SPToRGB8888(int[] rgb, byte[] yuv, int width, int height){
		
		int frameSize = width * height;
		
		for(int j = 0, yp = 0; j < height; j++){
			
			// yuv420sp memory 
			// y0y1y2y3y4y5y6y7
			// u0v0u1v1
			int uvp = frameSize + (j >> 1) * width;
			int u = 0;
			int v = 0;
			
			for(int i = 0; i < width; i++, yp++){
				
				int y = (0xff & ((int)yuv[yp]) ) - 16;
				
				if(y < 0) y = 0;
				
				// every two y get one uv
				if( (i & 1) == 0){
					v = (0xff & yuv[uvp++]) - 128;
					u = (0xff & yuv[uvp++]) - 128;
				}
				
				// 一个浮点计算转整形计算的技巧
				// R = Y + 1.4075 * (V - 128)
				// G = Y - 0.3455 * (U - 128) – 0.7169*(V - 128)
				// B = Y + 1.779  * (U - 128)
				int y1192 = y * 1192;
				int r = (y1192 + 1634 * v);
				int g = (y1192 - 833 * v - 400 * u);
				int b = (y1192 + 2066 * u);
				
				if(r < 0) r = 0; else if(r > 262143) r = 262143;
				if(g < 0) g = 0; else if(g > 262143) g = 262143;
				if(b < 0) b = 0; else if(b > 262143) b = 262143;
				
				rgb[yp] = 0xff000000 | (r << 6) & 0xff0000 | (g >> 2) & 0xff00 | (b >> 10) & 0xff;
			}
		}
	}
	
private static void initTable(){
	
	if(initialized == 0){
		
		initialized = 1;
		
		int m = 0;
		int n = 0;
		int tmp = 0;
		
		for(; m < 256; m++){
			mYTable[m] = 1192 * (m - 16);
			mCrTable[m] = 400 * (m - 128);
			mCbTable[m] = 833 * (m - 128);
		}
		
		for(m = 0; m < 256; m++){
			for(n = 0; n < 256; n++){
				
				tmp = 1192 * (m -16) + 2066 * (m - 128);
				if(tmp < 0) tmp = 0; else if(tmp > 262143) tmp = 262143;
			
				mYCrTable[m][n] = tmp;
				
				tmp = 1192 * (m -16) + 1634 * (m - 128);
				if(tmp < 0) tmp = 0; else if(tmp > 262143) tmp = 262143;
				
				mYCbTable[m][n] = tmp;
				
			}
		}
	}
}
	
	public static void covertYUV420SPToARGBTable(int[] rgb, byte[] yuv, int width, int height){
		
		int frameSize = width * height;
		
		initTable();
		
		for(int j = 0, yp = 0; j < height; j++){
			
			// yuv420sp memory 
			// y0y1y2y3y4y5y6y7
			// u0v0u1v1
			int uvp = frameSize + (j >> 1) * width;
			int u = 0;
			int v = 0;
			
			for(int i = 0; i < width; i++, yp++){
				
				int y = (0xff & ((int)yuv[yp]) ) - 16;
				
				if(y < 0) y = 0;
				
				// every two y get one uv
				if( (i & 1) == 0){
					v = (0xff & yuv[uvp++]) - 128;
					u = (0xff & yuv[uvp++]) - 128;
				}
				
				// 一个查表法的实现，简化乘法运算;

				int y1192 = mYTable[y];
				int r = mYCbTable[y][v];
				int g = y1192 - mCbTable[v] - mCrTable[u];
				int b = mYCrTable[y][u];
				
				if(r < 0) r = 0; else if(r > 262143) r = 263143;
				if(g < 0) g = 0; else if(g > 262143) g = 263143;
				if(b < 0) b = 0; else if(b > 262143) b = 263143;
				
				rgb[yp] = 0xff000000 | (r << 6) & 0xff0000 | (g >> 2) & 0xff00 | (b >> 10) & 0xff;
			}
		}
	}
	
	
	
	
	
	
	
	
	public static void covertYUV420SPToRGB565(int[] rgb, byte[] yuv, int width, int height){
		
		int frameSize = width * height;
		
		for(int j = 0, yp = 0; j < height; j++){
			
			// yuv420sp memory 
			// y0y1y2y3y4y5y6y7
			// u0v0u1v1
			int uvp = frameSize + (j >> 1) * width;
			int u = 0;
			int v = 0;
			
			for(int i = 0; i < width; i++, yp++){
				
				int y = (0xff & ((int)yuv[yp]) ) - 16;
				
				if(y < 0) y = 0;
				
				// every two y get one uv
				if( (i & 1) == 0){
					v = (0xff & yuv[uvp++]) - 128;
					u = (0xff & yuv[uvp++]) - 128;
				}
				
				// 一个浮点计算转整形计算的技巧
				// R = Y + 1.4075 * (V - 128)
				// G = Y - 0.3455 * (U - 128) – 0.7169*(V - 128)
				// B = Y + 1.779  * (U - 128)
				int y1192 = y * 1192;
				int r = (y1192 + 1634 * v);
				int g = (y1192 - 833 * v - 400 * u);
				int b = (y1192 + 2066 * u);
				
				if(r < 0) 
					r = 0; 
				else if(r > 262143) 
					r = 262143;
				
				if(g < 0) 
					g = 0; 
				else if(g > 262143) 
					g = 262143;
				
				if(b < 0) 
					b = 0; 
				else if(b > 262143) 
					b = 262143;
				
				rgb[yp] = 0xff000000 | ((r << 6) & 0xff0000) | ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
			}
		}
	}
	

}
