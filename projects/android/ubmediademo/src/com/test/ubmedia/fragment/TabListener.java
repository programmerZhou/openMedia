package com.test.ubmedia.fragment;

import android.app.ActionBar;
import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentTransaction;
import android.app.ActionBar.Tab;
import android.os.Bundle;

public  class TabListener<T extends Fragment> implements ActionBar.TabListener {
	
	private Fragment fragment;
	private final Activity activity;
	private final String tag;
	private final Class<T> instance;
	private final Bundle args;
	
	public TabListener(Activity activity, String tag, Class<T> clz) {
		
		this(activity, tag, clz, null);
	}
	
	public TabListener(Activity activity, String tag, Class<T> clz, Bundle args) {
		
		this.activity = activity;
		this.tag = tag;
		this.instance = clz;
		this.args = args;
	}
	
	public void onTabSelected(Tab tab, FragmentTransaction ft) {
		
		// Check if the fragment is already initialized
		if (fragment == null) {
		  
			// If not, instantiate and add it to the activity
			fragment = Fragment.instantiate(activity, instance.getName(), args);
			ft.add(android.R.id.content, fragment, tag);
		} else {
		  
			// If it exists, simply attach it in order to show it
			ft.attach(fragment);
		}
	}
	
	public void onTabUnselected(Tab tab, FragmentTransaction ft) {
		
		if (fragment != null) {
			// Detach the fragment, because another one is being attached
			ft.detach(fragment);
		}
	}
	
	public void onTabReselected(Tab tab, FragmentTransaction ft) {
		
		// User selected the already selected tab. Do nothing.
	}
}
