package com.example.alfred_bluetooth_controller;

import android.graphics.Canvas;
import android.view.SurfaceHolder;

public class RenderingThread extends Thread {

	private SurfaceHolder holder;
	private DrawSurface surface;
	private boolean running = false;

	public RenderingThread(SurfaceHolder surfaceHolder, DrawSurface surfaceView) {
		holder = surfaceHolder;
		surface = surfaceView;
	}

	public void setRunning(boolean run) {
		running = run;
	}

	@Override
	public void run() {
		while (running) {
			Canvas c = null;
			try {
				c = holder.lockCanvas(null);
				synchronized (holder) {
					surface.doDraw(c);
				}
			} finally {
				if (c != null) {
					holder.unlockCanvasAndPost(c);
				}
			}
		}
	}
}