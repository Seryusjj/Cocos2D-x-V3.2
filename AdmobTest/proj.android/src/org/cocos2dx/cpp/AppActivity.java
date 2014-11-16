/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.annotation.TargetApi;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;

public class AppActivity extends Cocos2dxActivity {

	private static AppActivity _appActiviy;
	private InterstitialAd intersticial;
	private static final String AD_UNIT_ID = "ca-app-pub-5951906881680430/1122906308";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		LinearLayout.LayoutParams adLayOut = new LinearLayout.LayoutParams(
				LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);

		intersticial = new InterstitialAd(this);

		intersticial.setAdUnitId(AD_UNIT_ID);

		AdRequest adRequest = new AdRequest.Builder().addTestDevice(
				"SH19HTR11632").build();

		intersticial.loadAd(adRequest);

		final Toast m= Toast.makeText(this, "The interstitial is loaded", Toast.LENGTH_SHORT);
		intersticial.setAdListener(new AdListener() {
		
			public void onAdLoaded() {
				m.show();
			}
		});

		_appActiviy = this;

	}

	public static void hideAd() {
		

	}

	public static void showAd() {

		if (_appActiviy.intersticial.isLoaded()) {
			
			
			_appActiviy.intersticial.show();
		}

	}

	@Override
	protected void onResume() {
		super.onResume();

	}

	@Override
	protected void onPause() {

		super.onPause();
	}

	@Override
	protected void onDestroy() {

		super.onDestroy();
	}

}