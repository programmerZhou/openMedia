package com.test.ubmedia.activity;


import java.io.InputStream;

import com.test.ubmedia.audio.AudioEngine;
import com.test.ubmedia.fragment.AudioMainMenuFragment;
import com.test.ubmedia.fragment.AudioSettingsMenuFragment;
import com.test.ubmedia.fragment.TabListener;
import com.test.ubmedia.fragment.VideoMainMenuFragment;
import com.test.ubmedia.fragment.VideoSettingsMenuFragment;
import com.test.ubmedia.media.MediaEngine;
import com.test.ubmedia.media.MenuStateProvider;
import com.test.ubmedia.video.VideoEngine;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;

public class MainActivity extends Activity implements MenuStateProvider{

	
	private Handler handler;
	private VideoEngine videoEngine = null;
	private AudioEngine audioEngine = null;
	public static MediaEngine mediaEngine = null;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		// Global settings.
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
	    mediaEngine = new MediaEngine(this);
	    
	    videoEngine = new VideoEngine(this);
	    audioEngine = new AudioEngine(this);
		
	    // Create action bar with all tabs.
	    ActionBar actionBar = getActionBar();
	    actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
	    actionBar.setDisplayShowTitleEnabled(false);

	    Tab tab = actionBar.newTab()
	        .setText("Video")
	        .setTabListener(new TabListener<VideoMainMenuFragment>(
	            this, "video", VideoMainMenuFragment.class));
	    
	    actionBar.addTab(tab);

	    tab = actionBar.newTab()
	        .setText("VideoSettings")
	        .setTabListener(new TabListener<VideoSettingsMenuFragment>(
	        		this, "VideoSetting", VideoSettingsMenuFragment.class));
	    
	    actionBar.addTab(tab);


	    tab = actionBar.newTab()
	        .setText("Audio")
	        .setTabListener(new TabListener<AudioMainMenuFragment>(
	            this, "audio", AudioMainMenuFragment.class));
	    
	    actionBar.addTab(tab);
		
	    tab = actionBar.newTab()
		    .setText("AudioSettings")
		    .setTabListener(new TabListener<AudioSettingsMenuFragment>(
		        this, "AudioSetting", AudioSettingsMenuFragment.class));
	    
		actionBar.addTab(tab);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	@Override
	public void onDestroy() {
	    
	    super.onDestroy();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			// Prevent app from running in the background.
			
			VideoMainMenuFragment main = (VideoMainMenuFragment)getFragmentManager().findFragmentByTag("main");
			main.stopAll();
			finish();
			
			return true;
		}
		
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public VideoEngine getVideoEngine() {
		// TODO Auto-generated method stub
		return videoEngine;
	}

	@Override
	public AudioEngine getAudioEngine() {
		// TODO Auto-generated method stub
		return audioEngine;
	}
	
	
}
