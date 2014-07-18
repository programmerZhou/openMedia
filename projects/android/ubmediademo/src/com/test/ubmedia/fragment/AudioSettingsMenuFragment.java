/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package com.test.ubmedia.fragment;

import com.test.ubmedia.activity.R;
import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Spinner;

public class AudioSettingsMenuFragment extends Fragment {

  private String TAG;

  @Override
  public View onCreateView(LayoutInflater inflater, ViewGroup container,
      Bundle savedInstanceState) {
    View v = inflater.inflate(R.layout.audiosettingsmenu, container, false);

    TAG = getResources().getString(R.string.tag);

    Spinner spAudioCodecType = (Spinner) v.findViewById(R.id.spAudioCodecType);
//    spAudioCodecType.setAdapter(new SpinnerAdapter(getActivity(),
//            R.layout.dropdownitems,
//            audioCodecsStrings,
//            inflater));
//    spAudioCodecType.setSelection(getEngine().audioCodecIndex());
//    spAudioCodecType.setOnItemSelectedListener(new OnItemSelectedListener() {
//        public void onItemSelected(AdapterView<?> adapterView, View view,
//            int position, long id) {
//          getEngine().setAudioCodec(position);
//        }
//        public void onNothingSelected(AdapterView<?> arg0) {
//          Log.d(TAG, "No setting selected");
//        }
//      });


    return v;
  }

  @Override
  public void onAttach(Activity activity) {
    super.onAttach(activity);

    // This makes sure that the container activity has implemented
    // the callback interface. If not, it throws an exception.
    try {
    
    } catch (ClassCastException e) {
      throw new ClassCastException(activity +
          " must implement MenuStateProvider");
    }
  }



}