package com.test.ubmedia.media;

import com.test.ubmedia.audio.AudioEngine;
import com.test.ubmedia.video.VideoEngine;

public interface MenuStateProvider{
	
	public VideoEngine getVideoEngine();
	
	public AudioEngine getAudioEngine();
}
