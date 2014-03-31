package com.example.alfred_bluetooth_controller;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class DrawSurface extends SurfaceView implements SurfaceHolder.Callback {

	private RenderingThread renderThread;

	private float mouseX, mouseY, radius = 100;
	private boolean mouseDown = false;
	
	private Paint paint;
	
	private MainActivity activity;
	
	public DrawSurface(MainActivity context) {
		super(context);
		activity = context;
		init();
	}

	protected void doDraw(Canvas canvas) {
		// super.onDraw(canvas);
		canvas.drawRGB(0, 0, 0);
		canvas.drawCircle(canvas.getWidth()/2,canvas.getHeight() * 3 / 4, radius, paint);
		canvas.drawCircle(mouseX, mouseY, radius, paint);
		
		float xper =  mouseX / canvas.getWidth();
		float multiplier = (float) ((((float) canvas.getHeight())*3/4 - mouseY) / canvas.getHeight() * 255 * 4 / 3 * (1 - Math.abs(.5 - xper)) * 2);
		int left = (int) ((1 - xper) * multiplier);
		int right = (int) ((xper) * multiplier);
		canvas.drawRect(0, canvas.getHeight() - left * 5, 100, canvas.getHeight(), paint);
		canvas.drawRect(canvas.getWidth() - 100, canvas.getHeight() - right * 5, canvas.getWidth(), canvas.getHeight(), paint);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// return super.onTouchEvent(event);
		int action = event.getAction();
		if (action == MotionEvent.ACTION_MOVE) {
			mouseX = event.getX();
			mouseY = event.getY();
			activity.moveAt(mouseX,mouseY);
		} else if (action == MotionEvent.ACTION_DOWN) {
			mouseX = event.getX();
			mouseY = event.getY();
			mouseDown = true;
			activity.startAt(mouseX,mouseY);
		} else if (action == MotionEvent.ACTION_UP) {
			mouseDown = false;
			activity.endAt(mouseX,mouseY);
		}

		return true;
	}
	
	private void init() {
		getHolder().addCallback(this);
		renderThread = new RenderingThread(getHolder(), this);

		setFocusable(true); // make sure we get key events
		paint = new Paint();
		paint.setARGB(64, 255, 255, 255);

	}

	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		renderThread.setRunning(true);
		renderThread.start();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		boolean retry = true;
		renderThread.setRunning(false);
		while (retry) {
			try {
				renderThread.join();
				retry = false;
			} catch (InterruptedException e) {
			}
		}
	}
}