package com.mvnordic.mvidexampleapp;

import com.mvnordic.mviddeviceconnector.DeviceSecurity;
import com.mvnordic.mviddeviceconnector.DeviceSecurity.DeviceSecurityListener.MVIDResponse;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class FragmentLoginActivity extends Activity {

	DeviceSecurity m_device_security = null;
	private Button m_login_button = null;
	private Button m_release_button = null;
	private Button m_check_login_button = null;
	
	private DeviceSecurity.DeviceSecurityListener device_security_listener = new DeviceSecurity.DeviceSecurityListener() {
	
		@Override
		public void onMVIDResponseReady(MVIDResponse response) {
			String toast = String.format("AI: %s\nREQUEST_ID: %s\nGOT ACCESS: %s",
					response.access_identifier,
					response.request_id,
					response.has_access ? "YES" : "NO" );
			Toast.makeText(getBaseContext(), toast, 
					   Toast.LENGTH_SHORT).show();
			if (response.has_access==false) {
				m_device_security.releaseDeviceRegistration();
			}
    		m_release_button.setVisibility(response.has_access ? Button.VISIBLE : Button.INVISIBLE);
    		m_login_button.setVisibility(response.has_access ? Button.INVISIBLE : Button.VISIBLE);
		}
		
	};

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fragment_login);
        
        m_device_security = new DeviceSecurity(this);
        m_device_security.addDeviceSecurityListener(device_security_listener);
        
        m_login_button = (Button) findViewById(R.id.frag_login_button);
        OnClickListener login_listener = new OnClickListener() {

			@Override
			public void onClick(View v) {
		        m_device_security.doLogin("product.ios.da.intowords",R.id.fragment_layout);
			}
        	
        };
        m_login_button.setOnClickListener(login_listener);

        m_check_login_button = (Button) findViewById(R.id.frag_checklogin_button);
        OnClickListener check_login_listener = new OnClickListener() {

			@Override
			public void onClick(View v) {
				m_device_security.checkLogin("product.ios.da.intowords");
			}
        	
        };
        m_check_login_button.setOnClickListener(check_login_listener);
        
		m_release_button = (Button) findViewById(R.id.frag_release_button);
		m_release_button.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				m_device_security.releaseDeviceRegistration();
	    		m_release_button.setVisibility(Button.INVISIBLE);
	            m_login_button.setVisibility(Button.VISIBLE);
			}
		});

        int req_id = m_device_security.checkLogin("product.ios.da.intowords");
		m_release_button.setVisibility(req_id>0 ? Button.VISIBLE : Button.INVISIBLE);
        m_login_button.setVisibility(req_id==0 ? Button.VISIBLE : Button.INVISIBLE);

	    // TODO Auto-generated method stub
	}

}
