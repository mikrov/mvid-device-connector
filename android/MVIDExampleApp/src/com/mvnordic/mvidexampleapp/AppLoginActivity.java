package com.mvnordic.mvidexampleapp;

import com.mvnordic.mviddeviceconnector.DeviceSecurity;
import com.mvnordic.mviddeviceconnector.DeviceSecurity.DeviceSecurityListener.MVIDResponse;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;
import android.os.Build;

public class AppLoginActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_app_login);

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.app_login, menu);
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

    
    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {

    	private DeviceSecurity m_device_security = null;
    	
        public PlaceholderFragment() {
        }

    	private DeviceSecurity.DeviceSecurityListener device_security_listener = new DeviceSecurity.DeviceSecurityListener() {

    		@Override
    		public void onMVIDResponseReady(MVIDResponse response) {
    			String toast = String.format("AI: %s\nREQUEST_ID: %s\nGOT ACCESS: %s",
    					response.access_identifier,
    					response.request_id,
    					response.has_access ? "YES" : "NO" );
    			Toast.makeText(getView().getContext(), toast, 
    					   Toast.LENGTH_SHORT).show();
    		}
    		
    	};

        
        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
        	
            m_device_security = new DeviceSecurity(this.getActivity());
            m_device_security.addDeviceSecurityListener(device_security_listener);
            
            View rootView = inflater.inflate(R.layout.fragment_app_login, container, false);
            Button login_button = (Button) rootView.findViewById(R.id.mvid_login_btn);
            
    		final Spinner spinner = (Spinner) rootView.findViewById(R.id.ai_spinner);
    		// Create an ArrayAdapter using the string array and a default spinner layout
    		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(rootView.getContext(),
    		        R.array.list_of_ais, android.R.layout.simple_spinner_item);
    		// Specify the layout to use when the list of choices appears
    		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
    		// Apply the adapter to the spinner
    		spinner.setAdapter(adapter);

            OnClickListener login_listener = new OnClickListener() {

				@Override
				public void onClick(View v) {
					m_device_security.doLogin(spinner.getSelectedItem().toString());
				}
            	
            };
            login_button.setOnClickListener(login_listener);

    		Button release_button = (Button) rootView.findViewById(R.id.release_device_hash);
    		release_button.setOnClickListener(new OnClickListener() {
    			public void onClick(View v) {
    				m_device_security.releaseDeviceRegistration();
    			}
    		});

            return rootView;
        }
    }

}
