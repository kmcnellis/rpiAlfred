package com.example.alfred_bluetooth_controller;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

import android.os.Bundle;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.app.DialogFragment;
import android.view.Menu;
import android.widget.ArrayAdapter;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Toast;



public class MainActivity extends Activity {
	
	int fingers_on = 0;
	
	BluetoothAdapter mBluetoothAdapter;
	BluetoothSocket mSocket;
	
	BT_Thread bt;
	
	long last_bt_send = 0;
	
	DrawSurface surface;
	
	Point screen_size = new Point();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		surface = new DrawSurface(this);
		setContentView(surface);
		
		getWindowManager().getDefaultDisplay().getSize(screen_size);
//		
		popToast("Started!");
//		
		configureBluetooth();
////		
//		addListeners();
		
		
	}
	
	public void configureBluetooth(){
		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if (mBluetoothAdapter != null) {
			// Device Supports Bluetooth
			
			if (mBluetoothAdapter.isEnabled()) {
				
				// Bluetooth is enabled on device
				
				
				// Get all bluetooth devices that are paired
				
				Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
				
				String[] device_names = new String[pairedDevices.size()];
				String[] devices_addresses = new String[pairedDevices.size()];
				
				// If there are paired devices
				if (pairedDevices.size() > 0) {
					
					int i = 0;
				    // Loop through paired devices
				    for (BluetoothDevice device : pairedDevices) {
				        // Add the name and address to an array adapter to show in a ListView
//				    	popToast(device.getName());
//				        mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
				    	device_names[i] = device.getName();
				    	devices_addresses[i] = device.getAddress();
				    	i++;
				    }
				}
				
				
				DialogFragment prompt = new BluetoothPrompt();
				
				Bundle args = new Bundle();
				
			    args.putStringArray("devices", device_names);
			    args.putStringArray("device_addresses", devices_addresses);
			    
			    prompt.setArguments(args);
				
				prompt.show(getFragmentManager(), "test");
				
				
				
			}else{
				popToast("[ERROR] Bluetooth Not Enabled");
			}
			
			
			
			
			
		}else{
			popToast("[ERROR] Device Does Not Support Bluetooth");
		}
	}
	
//	public void addListeners(){
//		final SeekBar sk1 = (SeekBar) findViewById(R.id.seekBar1);  
//		final SeekBar sk2 = (SeekBar) findViewById(R.id.seekBar2);  
//		
//		sk1.setOnSeekBarChangeListener(getSeekBarListener(0));
//		sk2.setOnSeekBarChangeListener(getSeekBarListener(1));
//		
//	}
	
//	public OnSeekBarChangeListener getSeekBarListener(final int motor){
//		 return new OnSeekBarChangeListener() {
//				
//			public void onStopTrackingTouch(SeekBar seekBar) {
//				fingers_on --;
//				if (fingers_on == 0){
//					// Last finger off, turn off the motor
//					bt.write(Protocol.MODE_STOP);
//				}
//			}
//			
//			public void onStartTrackingTouch(SeekBar seekBar) {
//				fingers_on ++;
//				if (fingers_on == 1){
//					// First finger on, send code to arduino to be in move mode
//					bt.write(Protocol.MODE_MOVE);
//				}
//			}
//			
//			public void onProgressChanged(SeekBar seekBar, int progress,
//					boolean fromUser) {
//				
//				if (System.currentTimeMillis() - last_bt_send > 100){
//					bt.write(Protocol.MotorMovement(motor, progress));
//					last_bt_send = System.currentTimeMillis();
//				}
//			}
//		};
//	}
	
	public void establishBluetoothConnection(String device_address){
		
		popToast("Device Selected, Address : " + device_address);
		
		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(device_address);
		
		try {
			mSocket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"));
			
			// TODO SHOULD NOT BE CONNECTING on UI THREAD!!!!
			// GET THIS CODE TO IT'S OWN THREAD
			
			bt = new BT_Thread(mSocket, this);
			bt.connectToSocket();
			
			popToast("Successfully Connected");
			
		} catch (IOException e) {
			popToast("Create Socket Failure");
			e.printStackTrace();
		}
		
		
		
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	public void popToast(String str) {
		Context context = getApplicationContext();
		Toast toast = Toast.makeText(context, str, Toast.LENGTH_SHORT);
		toast.show();
	}

	public void interpretBluetoothData(String line) {
		popToast("Recieved " + line);
	}
	
	public void changeMotorLevel(float x, float y){
		if (System.currentTimeMillis() - last_bt_send > 100){
			float xper =  x / screen_size.x;
			float multiplier = (float) ((((float) screen_size.y)*3/4 - y) / screen_size.y * 255 * 4 / 3 * (1 - Math.abs(.5 - xper)) * 2);
			int left = (int) ((.75 - xper) * multiplier);
			int right = (int) ((xper - .25) * multiplier);
			if (left > 0){
				if (left > 255){
					left = 255;
				}
				bt.write(Protocol.MotorMovement(0, left));
			}
			if (right > 0){
				if (right > 255){
					right = 255;
				}
				bt.write(Protocol.MotorMovement(1, right));
			}
			last_bt_send = System.currentTimeMillis();
		}
	}

	public void moveAt(float mouseX, float mouseY) {
		changeMotorLevel(mouseX,mouseY);
	}

	public void startAt(float mouseX, float mouseY) {
		bt.write(Protocol.MODE_MOVE);
		changeMotorLevel(mouseX,mouseY);
	}

	public void endAt(float mouseX, float mouseY) {
		bt.write(Protocol.MODE_STOP);
	}

}
