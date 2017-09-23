
package com.tope.esgui;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView;
import android.opengl.GLES20;
import android.opengl.GLES11Ext;
import android.opengl.GLUtils;
import android.util.Log;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener
{
    private native void DrawFrameNative();
    private native void SurfaceCreatedNative();
    private native void SurfaceChangedNative(int width, int height);

    private MainView mView;
    private int mHTex;
    private SurfaceTexture mSTexture;
    private boolean mUpdateST;
    public int lastw = 0, lasth = 0;
 
    MainRenderer( MainView view ) {
        mView = view;    
    }

    SurfaceTexture getTexture() { return mSTexture; }

    @Override
    public void onSurfaceCreated ( GL10 unused, EGLConfig config ) {
        //String extensions = GLES20.glGetString(GLES20.GL_EXTENSIONS);
        //Log.i("mr", "GLES20 extensions: " + extensions);
        //Assert.assertTrue(extensions.contains("OES_EGL_image_external"));

        mHTex = core.createTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES);
        mSTexture = new SurfaceTexture(mHTex);
        mSTexture.setOnFrameAvailableListener(this);
        mUpdateST = false;

        SurfaceCreatedNative();           
    }

    @Override
    public void onDrawFrame ( GL10 unused ) {
        if (mUpdateST) {
            mSTexture.updateTexImage();
            mUpdateST = false;
        }
        DrawFrameNative();
    }

    @Override
    public void onSurfaceChanged ( GL10 unused, int width, int height ) {
        lastw = width;
        lasth = height;
        SurfaceChangedNative(width, height);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        mUpdateST = true;
        mView.requestRender();
    }

    public void close() {
        mUpdateST = false;
        mSTexture.release();
    }

    public int getHTex() {
        return mHTex;
    }
}