package com.example.alfred_bluetooth_controller;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.app.DialogFragment;

public class BluetoothPrompt extends DialogFragment {
	
	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) {
		
		final String[] devices = getArguments().getStringArray("devices");
		final String[] addresses = getArguments().getStringArray("device_addresses");
	    AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
	    builder.setTitle("Choose Arduino Controller Bluetooth Connection");
	    builder.setItems(devices, new DialogInterface.OnClickListener() {
               public void onClick(DialogInterface dialog, int which) {
            	   ((MainActivity) getActivity()).establishBluetoothConnection(addresses[which]);
	           }
	    });
	    return builder.create();
	}
}
