
package com.esgui;

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

public class MainActivity extends Activity
	//extends AppCompatActivity
	//implements ActivityCompat.OnRequestPermissionsResultCallback 	
{
    protected MainView mView;
	
    private native void CreateObjectNative();
    private native void DeleteObjectNative();
    private native boolean IsInitsDoneNative();
	 
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

	    // call the native constructors to create an object
        CreateObjectNative();

        mView = new MainView(this);
        setContentView ( mView );
    }

	  @Override
    protected void onPause() {
        mView.onPause();
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // We are exiting the activity, let's delete the native objects
        DeleteObjectNative();
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
