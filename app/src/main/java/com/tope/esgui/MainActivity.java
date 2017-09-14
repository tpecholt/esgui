package com.tope.esgui;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.widget.Toast;

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

        CreateObjectNative();

        mView = new MainView(this);
        setContentView ( mView );
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

    public static int[] loadTexture(String uri)
    {
        try {
            int id = sActivity.mRes.getIdentifier(uri, "mipmap", sActivity.getPackageName());
            Bitmap bm = BitmapFactory.decodeResource(sActivity.mRes, id);
            return core.createTexture(bm);
        }
        catch (Exception e) {
            e.printStackTrace();
            return new int[]{0, 0, 0};
        }
    }
}
