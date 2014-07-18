package com.test.ubmedia.fragment;


import java.io.IOException;
import java.io.InputStream;

import com.test.ubmedia.activity.R;
import com.test.ubmedia.media.MediaEngine;
import com.test.ubmedia.media.MenuStateProvider;
import com.test.ubmedia.video.VideoEngine;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class VideoMainMenuFragment extends Fragment{

	
	private String TAG;

	private Button btStartStop;
	private TextView tvStats;
	
	private MenuStateProvider provider = null;
	private InputStream videoStream = null;
	
	// display and capture stream displays.
	private LinearLayout llDisplaySurface;
	private LinearLayout llCaptureSurface;
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		
		View v = inflater.inflate(R.layout.videomenu, container, false);

	    TAG = getResources().getString(R.string.tag);

	    llDisplaySurface = (LinearLayout) v.findViewById(R.id.llDisplayView);
	    llCaptureSurface = (LinearLayout) v.findViewById(R.id.llCaptureView);

	    Button btStartStop = (Button) v.findViewById(R.id.btStats);

	    btStartStop.setOnClickListener(new View.OnClickListener() {
	        public void onClick(View button) {

	        	try {
					videoStream = getResources().getAssets().open("predecode.h264");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	        	
	        	getVideoEngine().startVideoEngine(videoStream, 800, 600);
	        	
	        	setViews();
	        }
	    });
	    
	    tvStats = (TextView) v.findViewById(R.id.tvStats);

	    Button btSwitchCamera = (Button) v.findViewById(R.id.btSwitchCamera);
	    
	    btSwitchCamera.setText("Performance");

	    return v;
	  }
	

	  @Override
	  public void onAttach(Activity activity) {
	    super.onAttach(activity);

	    	// This makes sure that the container activity has implemented
	    	// the callback interface. If not, it throws an exception.
	    
	    	try{
	    		
	    		provider = (MenuStateProvider)activity;
	    	} catch (ClassCastException e) {
	    		throw new ClassCastException(activity + " must implement MenuStateProvider");
		    }
	  }
	  
	  
	  private VideoEngine getVideoEngine(){
		  
		  return provider.getVideoEngine();
	  }

	  // tvStats need to be updated on the UI thread.
	  public void newStats(final String stats) {
	    
		  getActivity().runOnUiThread(new Runnable() {
			  
			  public void run() {
				  tvStats.setText(stats);
			  }
	      });
	  }
	  
	  private void setViews() {
	    
		  SurfaceView displaySurfaceView = getVideoEngine().getDisplaySurfaceView();
		  
		  
		  
		  SurfaceView svCapture = getVideoEngine().getCaptureSurfaceView();
		  
		  if (displaySurfaceView != null) {
			  
			  llDisplaySurface.addView(displaySurfaceView);
		  }
		  
		  if (svCapture != null) {
			  
			  llCaptureSurface.addView(svCapture);
		  }
	  }
	  
	  
	  private void clearViews() {
	    
		  SurfaceView displaySurfaceView = null;//getEngine().getRemoteSurfaceView();
		  
		  if (displaySurfaceView != null) {
			  llDisplaySurface.removeView(displaySurfaceView);
		  }
		  
		  SurfaceView svCapture = null;//getEngine().getLocalSurfaceView();
		  
		  if (svCapture != null) {
			  llCaptureSurface.removeView(svCapture);
		  }
	  }
	
	  public void stopAll() {
	    
		  clearViews();
	  }
}
