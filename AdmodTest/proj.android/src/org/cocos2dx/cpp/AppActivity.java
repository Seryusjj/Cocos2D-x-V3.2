package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.RelativeLayout;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

public class AppActivity extends Cocos2dxActivity {

	private Cocos2dxGLSurfaceView mGLView;
	private static AppActivity _appActiviy;
	private AdView adView;
	private static final String AD_UNIT_ID = "ca-app-pub-2173642915566811/4742033684";



	public Cocos2dxGLSurfaceView onCreateView() {
		mGLView = super.onCreateView();
		return mGLView;

	}

	public void init() {
		super.init();
		this.mFrameLayout.removeView(mGLView);

	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);// aki llama al init

		RelativeLayout layout = new RelativeLayout(this);
		RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.MATCH_PARENT);
		layout.setLayoutParams(params);

		AdView admodView = createAdView();
		layout.addView(admodView);
		View gameView = createGameView();
		layout.addView(gameView);
		this.mFrameLayout.addView(layout);

		_appActiviy = this;

	}

	private AdView createAdView() {
		adView = new AdView(this);
		adView.setAdSize(AdSize.SMART_BANNER);
		adView.setAdUnitId(AD_UNIT_ID);
		AdRequest adRequest = new AdRequest.Builder().addTestDevice(AD_UNIT_ID)
				.build();
		adView.loadAd(adRequest);
		adView.setId(12345); // this is an arbitrary id, allows for relative
		// positioning in createGameView()
		RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		params.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);
		params.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);
		adView.setLayoutParams(params);
		adView.setBackgroundColor(Color.BLACK);
		return adView;
	}

	private View createGameView() {

		// create an auto touch for the ads
		// AdsAutoTouch.getAdsAutoTouch().add(new AdmodAutoTouch(adView));
		RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.WRAP_CONTENT);
		params.addRule(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.TRUE);
		params.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);
		params.addRule(RelativeLayout.ABOVE, adView.getId());
		this.mGLView.setLayoutParams(params);
		return this.mGLView;
	}

	public static void hideAd() {
		_appActiviy.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (_appActiviy.adView.isEnabled())
					_appActiviy.adView.setEnabled(false);
				if (_appActiviy.adView.getVisibility() != 4)
					_appActiviy.adView.setVisibility(View.INVISIBLE);
			}
		});

	}

	public static void showAd() {
		_appActiviy.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				if (!_appActiviy.adView.isEnabled())
					_appActiviy.adView.setEnabled(true);
				if (_appActiviy.adView.getVisibility() == 4)
					_appActiviy.adView.setVisibility(View.VISIBLE);
			}
		});

	}

	@Override
	protected void onResume() {
		super.onResume();
		if (adView != null) {
			adView.resume();
		}
	}

	@Override
	protected void onPause() {
		if (adView != null) {
			adView.pause();
		}
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		adView.destroy();
		super.onDestroy();
	}

}
