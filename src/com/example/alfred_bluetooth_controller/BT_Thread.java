package com.example.alfred_bluetooth_controller;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import android.bluetooth.BluetoothSocket;
import android.util.Log;

public class BT_Thread extends Thread {
		private final BluetoothSocket mmSocket;
		private InputStream mmInStream;
		private OutputStream mmOutStream;
		private final MainActivity activity;

		public BT_Thread(BluetoothSocket socket,MainActivity act) {
			Log.i("SEVE","Starting Socket");
			mmSocket = socket;
			activity = act;
		}
		
		public void connectToSocket(){
			Log.i("SEVE","Connecting to Socket");
			InputStream tmpIn = null;
			OutputStream tmpOut = null;
			
			try {
				mmSocket.connect();
			} catch (IOException e1) {
				activity.popToast("Couldn't connect to socket!");
			}
			
			Log.i("SEVE","Connected");

			// Get the input and output streams, using temp objects because
			// member streams are final
			try {
				tmpIn = mmSocket.getInputStream();
				tmpOut = mmSocket.getOutputStream();
			} catch (IOException e) {
				activity.popToast("Couldn't create streams!");
			}

			mmInStream = tmpIn;
			mmOutStream = tmpOut;
		}

		public void run() {
			
			connectToSocket();
			
			byte[] buffer = new byte[1024]; // buffer store for the stream
			int bytes; // bytes returned from read()
			BufferedReader r = new BufferedReader(new InputStreamReader(mmInStream));

			// Keep listening to the InputStream until an exception occurs
			while (true) {
				try {
					// Read from the InputStream
					//bytes = mmInStream.read(buffer);
					if (mmInStream.available() > 0){
						
						final String line = r.readLine();
						
						if (line != null){
						
							activity.runOnUiThread(new Runnable(){

								@Override
								public void run() {
									activity.interpretBluetoothData(line);
								}
							});
							
						}
						
					}

				} catch (IOException e) {
					break;
				}
			}
		}

//		 Call this from the main activity to send data to the remote device 
		public void write(byte[] bytes) {
			if (mmSocket.isConnected()){
				try {
					mmOutStream.write(bytes);
				} catch (IOException e) {
					logToast("Unable to send message: " + bytes.toString());
				}
			}else{
				logToast("No Bluetooth Connection");
			}
		}

//		 Call this from the main activity to shutdown the connection 
		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
				activity.popToast("Unable to recv message");
			}
		}
		
		public void logToast(final String msg){
			activity.runOnUiThread(new Runnable(){

				@Override
				public void run() {
					activity.popToast(msg);
				}
				
			});
		}
	}