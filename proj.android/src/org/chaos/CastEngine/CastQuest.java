/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.chaos.CastEngine;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.os.Bundle;
import android.util.Log;
import android.content.Context;
import android.app.Activity;
import android.app.AlarmManager;
import tv.ouya.console.*;
import tv.ouya.console.api.*;

public class CastQuest extends Cocos2dxActivity{

	private static Context context;
    private static CastQuest instance;
    private static Activity activity;
	
    protected void onCreate(Bundle savedInstanceState){    	
    	
    	
        instance = this;
		context = getApplicationContext();
        activity = (Activity)this;
        
        OuyaController.init(context);
    	
		super.onCreate(savedInstanceState);	
		
		Log.d("jCastQuest", "onCreate end");
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Log.d("jCastQuest", "onCreateView start");
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// Test should create stencil buffer
    	Log.d("jCastQuest", "onCreateView config");
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	Log.d("jCastQuest", "onCreateView end");
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     
}
