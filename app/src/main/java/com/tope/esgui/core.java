
package com.tope.esgui;

import android.graphics.*;
import android.graphics.Paint.*;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.os.Environment;
//import android.view.WindowManager;
//import android.view.Display;
//import android.content.Context;
import android.content.res.Resources;
import java.io.*;

class core
{
	static final int FONT_ATLAS_CELL_SIZE = 64;
	static final int FONT_ATLAS_COLS = 10;
	static final int FONT_ATLAS_POINT_SIZE = 48;
		
	public static int[] getScreenSize()
	{
		int w = Resources.getSystem().getDisplayMetrics().widthPixels;
		int h = Resources.getSystem().getDisplayMetrics().heightPixels;
		return new int[]{w, h};
	}

	public static int getDPI()
	{
		return Resources.getSystem().getDisplayMetrics().densityDpi;
	}

	//create a texture with 96 ascii characters in a regular grid 10x10
	public static int createFontAtlas(String fontName, boolean bold, boolean italic)
	{
		int width = FONT_ATLAS_COLS * FONT_ATLAS_CELL_SIZE; 
		Bitmap bitmap = Bitmap.createBitmap(width, width, Bitmap.Config.ARGB_4444);
		// get a canvas to paint over the bitmap
		Canvas canvas = new Canvas(bitmap);
		bitmap.eraseColor(0);
		canvas.drawARGB(0, 0, 255, 0);
		
		// Draw the text
		Paint textPaint = new Paint();
		int style = 0;
		if (bold) style |= Typeface.BOLD;
		if (italic) style |= Typeface.ITALIC;
		textPaint.setTypeface(Typeface.create(fontName, style));
		textPaint.setColor(Color.WHITE);
		textPaint.setTextSize(FONT_ATLAS_POINT_SIZE);
		textPaint.setAntiAlias(true);
		for (int i = 0; i < FONT_ATLAS_COLS*FONT_ATLAS_COLS - 2; ++i) {
			canvas.drawText(
					Character.toString((char)(' ' + i)), 
					(i % FONT_ATLAS_COLS) * FONT_ATLAS_CELL_SIZE, 
					(FONT_ATLAS_COLS - i / FONT_ATLAS_COLS) * FONT_ATLAS_CELL_SIZE, 
					textPaint
			);
		}

		/*for (int i = 0; i < 10; ++i)
			canvas.drawLine(0, i*CELL_SIZE, bitmap.getWidth(), i*CELL_SIZE, textPaint);*/

		/*FileOutputStream out = null;
		try {
			String path = Environment.getExternalStorageDirectory().toString();
			File file = new File(path, "hovno.PNG");
			out = new FileOutputStream(file);
			bitmap.compress(Bitmap.CompressFormat.PNG, 100, out); 
			out.close();			
		} catch (Exception e) {
			e.printStackTrace();
		}*/
		
		Matrix matrix = new Matrix();
		matrix.postScale(1, -1, bitmap.getWidth()/2, bitmap.getHeight()/2);
		Bitmap bmp = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
		bitmap.recycle();
		
		//Use the Android GLUtils to specify a two-dimensional texture image from our bitmap
		int[] hTex = new int[1];
		GLES20.glGenTextures(1, hTex, 0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, hTex[0]);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp, 0);

		bmp.recycle();
		return hTex[0];
	}

	public static float[] getFontSpacing(String fontName, boolean bold, boolean italic)
	{
		Paint p = new Paint();
		int style = 0;
		if (bold) style |= Typeface.BOLD;
		if (italic) style |= Typeface.ITALIC;
		p.setTypeface(Typeface.create(fontName, style));
		p.setTextSize(FONT_ATLAS_POINT_SIZE);
		float[] ret = new float[FONT_ATLAS_COLS*FONT_ATLAS_COLS];
		
		FontMetrics fm = p.getFontMetrics();
		/*Rect rect = new Rect();
		p.getTextBounds("Ag", 0, 2, rect);*/
		ret[ret.length - 2] = Math.abs((float)fm.top) / FONT_ATLAS_CELL_SIZE;
		ret[ret.length - 1] = Math.abs((float)fm.bottom) / FONT_ATLAS_CELL_SIZE;		
		
		for (int i = 0; i < FONT_ATLAS_COLS*FONT_ATLAS_COLS - 2; ++i) {
			//p.getTextWidths(Character.toString((char)(' ' + i)), 0, 1, tmp);
			float w = p.measureText(Character.toString((char)(' ' + i)), 0, 1);
			ret[i] = w / FONT_ATLAS_CELL_SIZE;
		}
		return ret;
	}
}
