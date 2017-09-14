
package com.tope.lotery;

import android.os.Bundle;
import android.view.Window;

public class MainActivity extends com.tope.esgui.MainActivity
{
    static {
        System.loadLibrary("native-lib");
    }
	   
    @Override
    public void onCreate(Bundle savedInstanceState) {
        // full screen & full brightness
        requestWindowFeature (Window.FEATURE_NO_TITLE);
        init(getResources());
        super.onCreate(savedInstanceState);
    }	
}
