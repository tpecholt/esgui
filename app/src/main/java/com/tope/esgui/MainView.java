
package com.tope.esgui;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.MotionEvent;

public class MainView extends GLSurfaceView {

	private native void OnTouchEventNative(int ev, float dx, float dy);

	private float lastX, lastY;
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

	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		int act = -1;
		switch (event.getAction()) {
			case MotionEvent.ACTION_DOWN:
				act = 0;
				break;
			case MotionEvent.ACTION_UP:
				act = 1;
				break;
			case MotionEvent.ACTION_MOVE:
				act = 2;
				break;
			default:
				return false;
		}
		//run in rendering thread so OpenGL calls will work
		class Tmp implements Runnable
		{
			float x, y;
			int act;
			Tmp(int act, float x, float y)
			{
				this.act = act;
				this.x = x;
				this.y = y;
			}
			@Override
			public void run() {
				OnTouchEventNative(act, x, y);
			}
		}
		queueEvent(new Tmp(act, event.getX(), event.getY()));
		return true;
	}
}