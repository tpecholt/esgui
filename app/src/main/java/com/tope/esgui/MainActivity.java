package com.tope.esgui;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
//import android.graphics.Camera;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.text.method.DigitsKeyListener;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.Toast;
import android.view.KeyEvent;
import android.hardware.Camera;

import java.io.IOException;
import java.util.List;

public class MainActivity extends Activity {

    protected static MainActivity sActivity;
    protected MainView mView;
    protected Resources mRes;
    private EditText mEditView;
    private Camera mCamera;
    private int mCameraId;

    private native void CreateObjectNative();
    private native void DeleteObjectNative();
    private native void OnBackPressedNative();
    private native void OnKeyNative(int c);
    private native void SetKbdHeightNative(int h);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sActivity = this;

        requestWindowFeature(Window.FEATURE_NO_TITLE);

        //set transparent status bar
        if (Build.VERSION.SDK_INT >= 21) {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            getWindow().setStatusBarColor(Color.TRANSPARENT);
            getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                    View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
        }
        else if (Build.VERSION.SDK_INT >= 19) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS, WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            //getWindow().setFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION, WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
            //getWindow().getDecorView().setBackgroundColor(Color.RED);
            //getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
            View view = new View(this);
            view.setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            view.getLayoutParams().height = getStatusBarHeight();
            ((ViewGroup) getWindow().getDecorView()).addView(view);
            view.setBackgroundColor(Color.RED);
        }

        //set view listener to monitor kbd height
        View mRootView = getWindow().getDecorView().findViewById(android.R.id.content);
        mRootView.getViewTreeObserver().addOnGlobalLayoutListener(
                new ViewTreeObserver.OnGlobalLayoutListener() {
                    public void onGlobalLayout(){
                        Rect r = new Rect();
                        View view = getWindow().getDecorView();
                        view.getWindowVisibleDisplayFrame(r);
                        r.top = 0; //status bar is transparent
                        int h = mView.mRenderer.lasth - r.height();
                        if (h >= 0)
                            SetKbdHeightNative(h);
                    }
                });

        CreateObjectNative();

        //setContentView( mView );
        //create shity layout with secret EditText in background
        FrameLayout lay = new FrameLayout(this);
        setContentView(lay, new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));

        mEditView = new EditText(this);
        mEditView.setLayoutParams(new FrameLayout.LayoutParams(1, 1));
        lay.addView(mEditView);

        mView = new MainView(this);
        mView.setLayoutParams(new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
        lay.addView(mView);
    }

    static public int getStatusBarHeight() {
        int result = 0;
        int resourceId = sActivity.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = sActivity.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    @Override
    protected void onDestroy() {
        openCamera(false);
        super.onDestroy();
        DeleteObjectNative();
    }

    @Override
    public void onBackPressed() {
        //super.onBackPressed();
        OnBackPressedNative();
    }

    public void init(Resources r)
    {
        mRes = r;
    }

    public static void setDarkStatusBar(boolean dark)
    {
        final boolean ddark = dark;
        sActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                final int lFlags = sActivity.getWindow().getDecorView().getSystemUiVisibility();
                sActivity.getWindow().getDecorView().setSystemUiVisibility(ddark ? (lFlags & ~View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR) : (lFlags | View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR));
            }
        });
    }

    public static void toastMessage(String msg)
    {
        final String tmp = msg;
        sActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(sActivity, tmp, Toast.LENGTH_SHORT).show();
            }
        });
    }

    public static int[] loadTexture(String folder, String name)
    {
        try {
            int id = sActivity.mRes.getIdentifier(name, folder, sActivity.getPackageName());
            /*Drawable drw = sActivity.mRes.getDrawable(id, null);
            BitmapDrawable bdrw = (BitmapDrawable)drw;
            Bitmap bm = bdrw.getBitmap();*/
            Bitmap bm = BitmapFactory.decodeResource(sActivity.mRes, id);
            return core.createTexture(bm);
        }
        catch (Exception e) {
            e.printStackTrace();
            return new int[]{0, 0, 0};
        }
    }

    public static void showKeyboard(int kb)
    {
        final int kbd = kb;
        sActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                InputMethodManager imm = (InputMethodManager) sActivity.getSystemService(sActivity.INPUT_METHOD_SERVICE);
                sActivity.mEditView.requestFocus();
                switch (kbd) {
                    case 0: //hide
                        imm.hideSoftInputFromWindow(sActivity.mEditView.getWindowToken(), 0);
                        break;
                    case 1: //all
                        sActivity.mEditView.setKeyListener(null);
                        imm.showSoftInput(sActivity.mEditView, InputMethodManager.SHOW_IMPLICIT);
                        break;
                    case 2: //number
                        sActivity.mEditView.setKeyListener(new DigitsKeyListener(false, false));
                        imm.showSoftInput(sActivity.mEditView, InputMethodManager.SHOW_IMPLICIT);
                        break;
                    case 3: //decimal
                        sActivity.mEditView.setKeyListener(new DigitsKeyListener(false, true));
                        imm.showSoftInput(sActivity.mEditView, InputMethodManager.SHOW_IMPLICIT);
                        break;
                }
            }
        });
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent KEvent)
    {
        int keyaction = KEvent.getAction();

        if(keyaction == KeyEvent.ACTION_DOWN)
        {
            int keycode = KEvent.getKeyCode();
            int keyunicode = KEvent.getUnicodeChar(KEvent.getMetaState() );
            if (keycode == KeyEvent.KEYCODE_DEL)
                keyunicode = 0x8;
            final int uni = keyunicode;
            sActivity.mView.queueEvent(new Runnable() {
                @Override
                public void run() {
                    OnKeyNative(uni);
                }
            });
        }

        return super.dispatchKeyEvent(KEvent);
    }

    public static int openCamera(boolean open)
    {
        final boolean op = open;
        final MainRenderer renderer = sActivity.mView.mRenderer;
        sActivity.runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                    try {
                        if (op) {
                            if (sActivity.mCamera == null) {
                                sActivity.mCameraId = 0;
                                sActivity.mCamera = Camera.open(sActivity.mCameraId);
                                sActivity.mCamera.setPreviewTexture(renderer.getTexture());
                                Camera.Parameters param = sActivity.mCamera.getParameters();
                                List<Camera.Size> psize = param.getSupportedPreviewSizes();
                                int maxRes = 0;
                                for (int i = 0; i < psize.size(); i++) {
                                    if (psize.get(i).width * psize.get(i).height > maxRes) {
                                        maxRes = psize.get(i).width * psize.get(i).height;
                                        param.setPreviewSize(psize.get(i).width, psize.get(i).height);
                                    }
                                }
                                param.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
                                sActivity.mCamera.setParameters(param);
                            }
                            sActivity.mCamera.startPreview();
                        }
                        else {
                            if (sActivity.mCamera != null) {
                                sActivity.mCamera.stopPreview();
                                sActivity.mCamera.release();
                                sActivity.mCamera = null;
                            }
                        }
                    } catch ( IOException ioe ) {
                        ioe.printStackTrace();
                    }
                }
            });
        return renderer.getHTex();
    }

    public static int getDisplayRotation() {
        int rotation = sActivity.getWindowManager().getDefaultDisplay()
                .getRotation();
        switch (rotation) {
            default:
                return 0;
            case Surface.ROTATION_90:
                return 90;
            case Surface.ROTATION_180:
                return 180;
            case Surface.ROTATION_270:
                return 270;
        }

    }
}
