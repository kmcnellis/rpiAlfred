package ehc.alfred;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.objdetect.CascadeClassifier;

import android.app.Activity;
import android.app.DialogFragment;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Toast;
import ehc.alfred.util.SystemUiHider;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 * 
 * @see SystemUiHider
 */
public class AlfredUIActivity extends Activity implements CvCameraViewListener2 {

	private static final String TAG = "AlfredUI::Activity";

	// UI
	private static final Scalar FACE_RECT_COLOR = new Scalar(15, 15, 215, 255);
	private CameraBridgeViewBase mOpenCvCameraView;
	private ImageButton soda_button1;
	private ImageButton soda_button2;
	private ImageButton soda_button3;
	private ImageButton soda_button4;

	private ImageButton sodaButtonMap(int button_index) {
		switch (button_index) {
		case 1:
			return soda_button1;
		case 2:
			return soda_button2;
		case 3:
			return soda_button3;
		case 4:
			return soda_button4;
		default:
			return null;
		}
	}

	// Arduino Sensors
	private int IR_SENSOR;
	private int ULTRA_SOUND_SENSOR;
	private int DISPENSING_COMPLETE_SENSOR;

	// Bluetooth
	private BluetoothAdapter bAdapter;
	private BluetoothSocket bSocket;
	private OutputStream bOutput;
	private InputStream bInput;
	private boolean dispensing = false;
	private boolean bluetoothConnected = false;
	// found default online
	private static final UUID SPP_UUID = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB");
	private static final int DISCOVERY_REQUEST = 1;

	// OpenCV
	private Mat mRgba;
	private Mat mGray;
	private File mCascadeFile;
	private CascadeClassifier mJavaDetector;
	private float mRelativeFaceSize = 0.2f;
	private int mAbsoluteFaceSize = 0;
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS: {
				Log.i(TAG, "OpenCV loaded successfully");

				try {
					// load cascade file from application resources
					InputStream is = getResources().openRawResource(
							R.raw.lbpcascade_frontalface);
					File cascadeDir = getDir("cascade", Context.MODE_PRIVATE);
					mCascadeFile = new File(cascadeDir,
							"lbpcascade_frontalface.xml");
					FileOutputStream os = new FileOutputStream(mCascadeFile);

					byte[] buffer = new byte[4096];
					int bytesRead;
					while ((bytesRead = is.read(buffer)) != -1) {
						os.write(buffer, 0, bytesRead);
					}
					is.close();
					os.close();

					mJavaDetector = new CascadeClassifier(
							mCascadeFile.getAbsolutePath());
					if (mJavaDetector.empty()) {
						Log.e(TAG, "Failed to load cascade classifier");
						mJavaDetector = null;
					} else
						Log.i(TAG, "Loaded cascade classifier from "
								+ mCascadeFile.getAbsolutePath());

					cascadeDir.delete();

				} catch (IOException e) {
					e.printStackTrace();
					Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
				}

				mOpenCvCameraView.enableView();
			}
				break;
			default: {
				super.onManagerConnected(status);
			}
				break;
			}
		}
	};

	// Bluetooth Receiver
	/*final BroadcastReceiver mReceiver = new BroadcastReceiver() {
		public void onReceive(Context context, Intent intent) {
			Log.i("SEVE","Broadcast Received");
			if (BluetoothDevice.ACTION_FOUND.equals(intent.getAction())) {
				bDevice = intent
						.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
				if (bDevice.getName().equals("BT UART")) {
					bAdapter.cancelDiscovery();
					try {
						bSocket = bDevice
								.createRfcommSocketToServiceRecord(SPP_UUID);
						bSocket.connect();
						logToast("Bluetooth Connected");
					} catch (IOException e) {
						// TODO Auto-generated catch block
						logToast("Socket Connection Failed");
						popToast(e.getMessage());
						e.printStackTrace();
					}

				}
			}
		};
	};
*/
	// http://developer.android.com/guide/topics/connectivity/bluetooth.html
	private class BT_Thread extends Thread {
		private final BluetoothSocket mmSocket;
		private final InputStream mmInStream;
		private final OutputStream mmOutStream;

		public BT_Thread(BluetoothSocket socket) {
			mmSocket = socket;
			InputStream tmpIn = null;
			OutputStream tmpOut = null;

			// Get the input and output streams, using temp objects because
			// member streams are final
			try {
				tmpIn = socket.getInputStream();
				tmpOut = socket.getOutputStream();
			} catch (IOException e) {
			}

			mmInStream = tmpIn;
			mmOutStream = tmpOut;
		}

		public void run() {
			byte[] buffer = new byte[1024]; // buffer store for the stream
			int bytes; // bytes returned from read()

			// Keep listening to the InputStream until an exception occurs
			while (true) {
				try {
					// Read from the InputStream
					bytes = mmInStream.read(buffer);

				} catch (IOException e) {
					break;
				}
			}
		}

//		 Call this from the main activity to send data to the remote device 
		public void write(byte[] bytes) {
			try {
				mmOutStream.write(bytes);
			} catch (IOException e) {
				logToast("Unable to send message: " + bytes.toString());
			}
		}

//		 Call this from the main activity to shutdown the connection 
		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
				logToast("Unable to recv message");
			}
		}
	}
/*
	private void bSocketSend(String msg) {
		if (bSocket.isConnected()) {
			byte[] byteMsg = msg.getBytes();
			byte[] socketMsg = new byte[byteMsg.length + 1];
			for (int i = 0; i < byteMsg.length; i++) {
				socketMsg[i] = byteMsg[i];
			}
			socketMsg[socketMsg.length - 1] = '\0';
			try {
				bOutput = bSocket.getOutputStream();
				bOutput.write(socketMsg);
			} catch (IOException e) {
				e.printStackTrace();
			}
		} else {
			logToast("Socket Disconnected");
		}
	}
*/
	
	private void bluetoothInitialization(){
		
		Log.i("SEVE","Initializing Bluetooth");
		
		bAdapter = BluetoothAdapter.getDefaultAdapter();
		if (bAdapter != null) {
			// Device Supports Bluetooth
			
			if (bAdapter.isEnabled()) {
				
				// Bluetooth is enabled on device
				
				
				// Get all bluetooth devices that are paired
				
				Set<BluetoothDevice> pairedDevices = bAdapter.getBondedDevices();
				
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
	
	public void establishBluetoothConnection(String device_address){
		
		popToast("Device Selected, Address : " + device_address);
		
		BluetoothDevice device = bAdapter.getRemoteDevice(device_address);
		
		try {
			bSocket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"));
			
			// TODO SHOULD NOT BE CONNECTING on UI THREAD!!!!
			// GET THIS CODE TO IT'S OWN THREAD
			
			bSocket.connect();
			
			bOutput = bSocket.getOutputStream();
			bInput = bSocket.getInputStream();
			
			bluetoothConnected = true;
			
			popToast("Successfully Connected");
			
		} catch (IOException e) {
			popToast("Create Socket Failure");
			e.printStackTrace();
		}
		
		
		
	}
	
	private void sendBluetoothMessage(String str){
		Log.i("SEVE","Sending Message : " + str);
		
		if (bluetoothConnected && bSocket != null && bSocket.isConnected()){
			
			str += "\n";
			
			try {
				bOutput.write(str.getBytes());
			} catch (IOException e) {
				popToast("Couldn't send message!");
			}
			
		}
		
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Log.i("SEVE","CREATED");

		setContentView(R.layout.activity_alfred_ui);
		bluetoothInitialization();

		// final View controlsView =
		// findViewById(R.id.fullscreen_content_controls);
		// final View contentView = findViewById(R.id.fullscreen_content);
		mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.cv_activity_surface_view);
		mOpenCvCameraView.setCvCameraViewListener(this);
		soda_button1 = (ImageButton) findViewById(R.id.soda_button1);
		soda_button2 = (ImageButton) findViewById(R.id.soda_button2);
		soda_button3 = (ImageButton) findViewById(R.id.soda_button3);
		soda_button4 = (ImageButton) findViewById(R.id.soda_button4);
		soda_button1.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				onSodaButton((ImageButton) v, 1);
			}
		});
		soda_button2.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				onSodaButton((ImageButton) v, 2);
			}
		});
		soda_button3.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				onSodaButton((ImageButton) v, 3);
			}
		});
		soda_button4.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				onSodaButton((ImageButton) v, 4);
			}
		});
	}
/*
	private void bluetoothInitialization() {
		bAdapter = BluetoothAdapter.getDefaultAdapter();
		startActivityForResult(new Intent(
				BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE),
				DISCOVERY_REQUEST);
		IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		registerReceiver(mReceiver, filter);
		bAdapter.startDiscovery();
		
		
	}*/

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
	}

	@Override
	public void onPause() {
		super.onPause();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	@Override
	public void onResume() {
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_5, this,
				mLoaderCallback);
	}

	public void onDestroy() {
		super.onDestroy();
		mOpenCvCameraView.disableView();
	}

	public void onCameraViewStarted(int width, int height) {
		mGray = new Mat();
		mRgba = new Mat();
	}

	public void onCameraViewStopped() {
		mGray.release();
		mRgba.release();
	}

	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {

		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();

		if (mAbsoluteFaceSize == 0) {
			int height = mGray.rows();
			if (Math.round(height * mRelativeFaceSize) > 0) {
				mAbsoluteFaceSize = Math.round(height * mRelativeFaceSize);
			}
		}

		MatOfRect faces = new MatOfRect();

		if (mJavaDetector != null)
			/*
			 * CV_HAAR_DO_CANNY_PRUNING 1 CV_HAAR_SCALE_IMAGE 2
			 * CV_HAAR_FIND_BIGGEST_OBJECT 4 CV_HAAR_DO_ROUGH_SEARCH 8
			 */
			mJavaDetector.detectMultiScale(mGray, faces, 1.1, 2, 2, new Size(
					mAbsoluteFaceSize, mAbsoluteFaceSize), new Size());
		Rect[] facesArray = faces.toArray();
		for (int i = 0; i < facesArray.length; i++)
			Core.rectangle(mRgba, facesArray[i].tl(), facesArray[i].br(),
					FACE_RECT_COLOR, 3);

		return mRgba;
	}

	private void onSodaButton(ImageButton button, int button_index) {
		if (!dispensing){
			dispensing = true;
			logToast("Soda Button: " + button_index);
			// Send message to arduino to begin dispensing soda
			sendBluetoothMessage("1," + Integer.toString(button_index));
			
			//TODO remove
			// For now, stop dispensing after 2 seconds
			final Handler handler = new Handler();
			handler.postDelayed(new Runnable() {
			  @Override
			  public void run() {
				  sendBluetoothMessage("0,0");
				  dispensing = false;
			  }
			}, 2000);
		}
		
		
		// button.setEnabled(false);
	}

	private void logToast(String str) {
		popToast(str);
		Log.i(TAG, str);
	}

	private void popToast(String str) {
		Context context = getApplicationContext();
		Toast toast = Toast.makeText(context, str, Toast.LENGTH_SHORT);
		toast.show();
	}
}
