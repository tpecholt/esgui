
package com.tope.esgui;

import android.opengl.GLSurfaceView;
import android.opengl.GLES20;
import android.opengl.GLES11Ext;
import android.opengl.GLUtils;
import android.util.Log;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainRenderer implements GLSurfaceView.Renderer 
{
    private native void DrawFrameNative();
    private native void SurfaceCreatedNative();
    private native void SurfaceChangedNative(int width, int height);

    private MainView mView;
 
    MainRenderer( MainView view ) {
        mView = view;    
    }
   
    public void onSurfaceCreated ( GL10 unused, EGLConfig config ) {
        //String extensions = GLES20.glGetString(GLES20.GL_EXTENSIONS);
        //Log.i("mr", "GLES20 extensions: " + extensions);
        //Assert.assertTrue(extensions.contains("OES_EGL_image_external"));

        SurfaceCreatedNative();           
    }
   
    public void onDrawFrame ( GL10 unused ) {
        DrawFrameNative();	    
    }
   
    public void onSurfaceChanged ( GL10 unused, int width, int height ) {
        SurfaceChangedNative(width, height);
    }  

    public void close() {
    }
}