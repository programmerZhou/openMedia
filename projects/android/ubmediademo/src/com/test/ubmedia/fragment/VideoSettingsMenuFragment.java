package com.test.ubmedia.fragment;


import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import com.test.ubmedia.activity.R;
import com.test.ubmedia.media.MenuStateProvider;
import com.test.ubmedia.video.VideoEngine;

public class VideoSettingsMenuFragment extends Fragment
    implements RadioGroup.OnCheckedChangeListener {

	private String TAG;

	private EditText etRemoteIp;
  	
	private MenuStateProvider provider = null;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
	      Bundle savedInstanceState) {
	    
		View v = inflater.inflate(R.layout.videosettingsmenu , container, false);
	
	    TAG = getResources().getString(R.string.tag);
	
	    CheckBox cbVideoReceive = (CheckBox) v.findViewById(R.id.cbVideoReceive);
	
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
	 			throw new ClassCastException(activity +
	 					" must implement MenuStateProvider");
	 		}
 	 }

	 public VideoEngine getVideoEngine(){
		 
		 return provider.getVideoEngine();
	 }
	 
	 @Override
	 public void onCheckedChanged(RadioGroup group, int checkedId) {
		 // TODO Auto-generated method stub
		
		
		switch (checkedId) {
		case R.id.rOpenGl:
			getVideoEngine().setViewSelection(getResources().getInteger(R.integer.openGl));
		    break;
		case R.id.rSurfaceView:
		    getVideoEngine().setViewSelection(
		          getResources().getInteger(R.integer.surfaceView));
		    break;
		case R.id.rMediaCodec:
		    getVideoEngine().setViewSelection(
		          getResources().getInteger(R.integer.mediaCodec));
		    break;
		default:
			break;
		}
	 }


 
}