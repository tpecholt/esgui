
package com.tope.lotery;

import android.graphics.Color;
import android.os.Bundle;
import android.view.Window;

public class MainActivity extends com.tope.esgui.MainActivity
{
    static {
        System.loadLibrary("native-lib");
    }
	   
    @Override
    public void onCreate(Bundle savedInstanceState) {
        init(getResources());
        super.onCreate(savedInstanceState);
    }	
}
