package com.mvnordic.mvidexampleapp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button activity_login_button = (Button) findViewById(R.id.activity_login);
        Button fragment_login_button = (Button) findViewById(R.id.fragment_login);
        final Activity main_activity = this;
		activity_login_button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(main_activity,com.mvnordic.mvidexampleapp.AppLoginActivity.class);
				main_activity.startActivity(intent);
			}
		});
		
		fragment_login_button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intent = new Intent(main_activity,com.mvnordic.mvidexampleapp.FragmentLoginActivity.class);
				main_activity.startActivity(intent);
			}
		});
	    // TODO Auto-generated method stub
	}

}
