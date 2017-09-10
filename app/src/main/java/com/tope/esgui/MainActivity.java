package com.tope.esgui;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    protected MainView mView;

    private native void CreateObjectNative();
    private native void DeleteObjectNative();

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
}
