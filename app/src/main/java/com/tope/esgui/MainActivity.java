package com.tope.esgui;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    protected MainView mView;

    private native void CreateObjectNative();
    private native void DeleteObjectNative();
    private native void OnBackPressedNative();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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
}
