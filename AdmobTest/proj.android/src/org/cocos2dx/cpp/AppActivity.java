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

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;
import android.view.WindowManager;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

public class AppActivity extends Cocos2dxActivity {

	private static int numClick = 0;
	private static boolean haveToLoad = true;
	private static AppActivity _appActiviy;
	private InterstitialAd interstitial;
	private AdRequest adRequest;
	private static final String AD_UNIT_ID = "ca-app-pub-5951906881680430/1122906308";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		interstitial = new InterstitialAd(this);

		interstitial.setAdUnitId(AD_UNIT_ID);

		adRequest = new AdRequest.Builder().addTestDevice("SH19HTR11632")
				.addTestDevice("20080411").build();

		interstitial.loadAd(adRequest);
		interstitial.setAdListener(new AdListener() {
			public void onAdClosed() {
				// se consume el anuncio entonces recagar
				if (numClick < 2) {
					interstitial.loadAd(adRequest);
				}
			}

			public void onAdLeftApplication() {
				// por ejemplo 2 click por dia para no ser baneado
				numClick++;
			}
		});

		_appActiviy = this;

	}

	public static void preloadInterstitialAd() {
		// _appActiviy.runOnUiThread(new Runnable() {
		// public void run() {
		// if (haveToLoad) {
		//
		//
		// _appActiviy.interstitial.loadAd(_appActiviy.adRequest);
		// haveToLoad = false;
		// }
		// }
		// });
	}

	public static void showAd() {

		_appActiviy.runOnUiThread(new Runnable() {
			public void run() {
				if (_appActiviy.interstitial.isLoaded()) {
					_appActiviy.interstitial.show();

				}
			}
		});

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