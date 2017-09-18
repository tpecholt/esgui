
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
	public static int createFontAtlas(String fontName, boolean bold, boolean italic, boolean underline)
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
		if (underline) textPaint.setUnderlineText(true);
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

		int[] hTex = createTexture(bitmap);
		bitmap.recycle();
		return hTex[0];
	}

	public static float[] getFontSpacing(String fontName, boolean bold, boolean italic, boolean underline)
	{
		Paint p = new Paint();
		int style = 0;
		if (bold) style |= Typeface.BOLD;
		if (italic) style |= Typeface.ITALIC;
		p.setTypeface(Typeface.create(fontName, style));
		p.setTextSize(FONT_ATLAS_POINT_SIZE);
		if (underline) p.setUnderlineText(true);
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

	public static int[] createTexture(Bitmap bmp)
	{
		Matrix matrix = new Matrix();
		matrix.postScale(1, -1, bmp.getWidth()/2, bmp.getHeight()/2);
		Bitmap bmp2 = Bitmap.createBitmap(bmp, 0, 0, bmp.getWidth(), bmp.getHeight(), matrix, true);

		int[] hTex = new int[3];
		hTex[1] = bmp2.getWidth();
		hTex[2] = bmp2.getHeight();
		GLES20.glGenTextures(1, hTex, 0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, hTex[0]);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp2, 0);
		bmp2.recycle();
		return hTex;
	}
}
