
package com.esgui;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.SurfaceHolder;

public class MainView extends GLSurfaceView {

    protected MainRenderer mRenderer;

    public MainView(Context context) {
        super(context);

        try {
            // Create GLES context. Even though we are specifying OpenGL ES 2, it will try to
            // create the highest possible context on a phone
            setEGLContextClientVersion(2);
            setEGLConfigChooser(8, 8, 8, 8, 16, 0);

            // set our custom Renderer for drawing on the created SurfaceView
            mRenderer = new MainRenderer(this);
            setRenderer(mRenderer);
			
            // calls onDrawFrame(...) continuously
            setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        } catch (Exception e) {

            // Trouble, something's wrong!
            Log.e("GLESNativeAndroid", "Unable to create GLES context!", e);

        }

    }

    public void surfaceCreated ( SurfaceHolder holder ) {
        super.surfaceCreated ( holder );
	  }
 
	  public void surfaceDestroyed ( SurfaceHolder holder ) {
        mRenderer.close();
        super.surfaceDestroyed ( holder );
	  }
 
	  public void surfaceChanged ( SurfaceHolder holder, int format, int w, int h ) {
        super.surfaceChanged ( holder, format, w, h );
	  }

}