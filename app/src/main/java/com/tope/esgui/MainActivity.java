package com.tope.esgui;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.tope.lotery.R;

import java.lang.reflect.Field;

public class MainActivity extends Activity {

    protected MainView mView;
    protected Resources mRes;
    protected static MainActivity sActivity;

    private native void CreateObjectNative();
    private native void DeleteObjectNative();
    private native void OnBackPressedNative();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sActivity = this;

        requestWindowFeature(Window.FEATURE_NO_TITLE);

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

        CreateObjectNative();

        mView = new MainView(this);
        setContentView ( mView );
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
}
