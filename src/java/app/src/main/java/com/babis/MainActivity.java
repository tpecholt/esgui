
package com.babis;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Bundle;
import android.hardware.Camera;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.view.Window; 
import android.view.WindowManager;
import android.content.Context;
import android.content.pm.PackageManager;
//import android.support.v4.app.ActivityCompat;
//import android.support.v7.app.AppCompatActivity;

import java.lang.ref.WeakReference;
import java.io.IOException;

public class MainActivity extends com.esgui.MainActivity
	//extends AppCompatActivity
	//implements ActivityCompat.OnRequestPermissionsResultCallback 	
{
    private WakeLock mWL;
    private Camera mCamera = null;

	// load libGLESNative.so since it has all the native functions
    static {
        System.loadLibrary("GLESNative");
    }
	   
    @Override
    public void onCreate(Bundle savedInstanceState) {
        // full screen & full brightness
        requestWindowFeature (Window.FEATURE_NO_TITLE);
        
        super.onCreate(savedInstanceState);

        /*if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
          if (ContextCompat.checkSelfPermission(this, android.Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(new String[] {android.Manifest.permission.CAMERA}, 1);
        }*/

        /*getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        mWL = ((PowerManager)getSystemService ( Context.POWER_SERVICE )).newWakeLock(PowerManager.FULL_WAKE_LOCK, "WakeLock");
        mWL.acquire();*/        
    }

	/*@Override
	public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);

		switch (requestCode) {
			case CAMERA_PERMISSION_REQUEST_CODE:
				// If request is cancelled, the result arrays are empty.
				if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
					//Start your camera handling here
				} else {
					AppUtils.showUserMessage("You declined to allow the app to access your camera", this);
				}
		}
	}*/

    @Override
    protected void onPause() {
        /*if ( mWL.isHeld() )
        mWL.release();*/
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        /*if(!mWL.isHeld()) 
          mWL.acquire();*/
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();        
    }


    // this task simply waits till all inits are completed in native code
    // then it displays the GLES version on the screen
    /*class AsyncDisplayText extends AsyncTask<Void, String, Integer> {

        @Override
        // this function executes on a background thread
        // hence we cannot make GL calls or manipulate UI elements here
        protected Integer doInBackground(Void... params) {

            // keep polling to check if native objects are initialized
            while(!IsInitsDoneNative()) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            return 0;
        }

        @Override
        // this function executes on the UI thread and we can modify UI elements
        protected void onPostExecute(Integer result) {

            // GLES version is displayed on the screen
            DisplayGLVersion(GetGLESVersionNative());

        }
    }*/
	
}
