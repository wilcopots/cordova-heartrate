package nl.trimbos.eva;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Arrays;

import android.content.pm.PackageManager;
import android.Manifest;
import android.util.Log;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.SystemClock;
import android.view.WindowManager;

import androidx.annotation.NonNull;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.PermissionHelper;
import org.apache.cordova.PluginResult;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.happitech.heartbeatandroid.Error;
import com.happitech.heartbeatandroid.ExportType;
import com.happitech.heartbeatandroid.FrequencyHRV;
import com.happitech.heartbeatandroid.GraphSettings;
import com.happitech.heartbeatandroid.HeartListener;
import com.happitech.heartbeatandroid.HeartMonitor;
import com.happitech.heartbeatandroid.HeartRate;
import com.happitech.heartbeatandroid.Status;
import com.happitech.heartbeatandroid.TimeHRV;
import com.happitech.heartbeatandroid.Warning;

import org.jetbrains.annotations.NotNull;

public class HeartbeatPlugin extends CordovaPlugin implements HeartListener {

  private static final String TAG = "HeartbeatPlugin";
  private HeartMonitor monitor;
  private CallbackContext mainCallback;
  private List<PluginResult> resultQueue;
  private int measureTime = 60;

  // private Handler resultHandler = new Handler();
  // private boolean isSendingResult;
  private HashMap<String, String> resultHashMap = new HashMap<String, String>();

  @Override
  public void initialize(CordovaInterface cordova, CordovaWebView webView) {
    Log.i(TAG, "initialize HeartbeatPlugin");
    super.initialize(cordova, webView);
    resultQueue = new ArrayList<PluginResult>();
    monitor = new HeartMonitor(cordova.getActivity(), this, measureTime);
    sendResultQueue();
  }

  @Override
  public boolean execute(String action, final JSONArray args, final CallbackContext callbackContext) throws JSONException {
    if (action.equals("start")) {
      start(callbackContext);
      return true;
    } else if (action.equals("stop")) {
      stop(callbackContext);
      return true;
    } else if (action.equals("setMeasureTime")) {
      setMeasureTime(args, callbackContext);
      return true;
    } else if (action.equals("checkPermissions")) {
      checkPermissions(callbackContext);
      return true;
    }

    return false;
  }

  /**
   * Start the plugin, keep the callbackContext for future events
   * @param callbackContext
   */
  protected void start(CallbackContext callbackContext) {
    Log.i(TAG, "start");
    mainCallback = callbackContext;
    PluginResult result = new PluginResult(PluginResult.Status.OK, "");
    result.setKeepCallback(true);
    callbackContext.sendPluginResult(result);
    monitor.startMeasuring();

    keepScreenAwake();
  }

  /**
   * Stop the plugin
   * @param callbackContext
   */
  protected void stop(final CallbackContext callbackContext) {
    Log.i(TAG, "stop");
    final PluginResult result = new PluginResult(PluginResult.Status.OK, "");
    monitor.stopMeasuring();

    cordova.getThreadPool().execute(new Runnable() {
      public void run() {
        callbackContext.sendPluginResult(result); // Thread-safe.
      }
    });
    allowScreenToSleep();
  }

  /**
   * Sets the measure time
   * @param args
   * @param callbackContext
   */
  protected void setMeasureTime(JSONArray args, final CallbackContext callbackContext) {
    Log.i(TAG, "setMeasureTime");
    try {
      measureTime = args.getInt(0);
      monitor = new HeartMonitor(cordova.getActivity(), this, measureTime);
      final PluginResult result = new PluginResult(PluginResult.Status.OK, "");
      cordova.getThreadPool().execute(new Runnable() {
        public void run() {
          callbackContext.sendPluginResult(result); // Thread-safe.
        }
      });

    } catch (JSONException e) {
      Log.e(TAG, "could not serialize result for callback");
      callbackContext.error("could not serialize result for callback");
    }
    Log.d(TAG, "Set measure time: " + measureTime);
  }

  private void keepScreenAwake() {
    try {
      cordova.getActivity().runOnUiThread(
        new Runnable() {
          public void run() {
            cordova.getActivity().getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
          }
        }
      );
    } catch (Exception e) {
      Log.e(TAG, e.getMessage());
    }
  }

  private void allowScreenToSleep() {
    try {
      cordova.getActivity().runOnUiThread(
        new Runnable() {
          public void run() {
            cordova.getActivity().getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
          }
        }
      );
    } catch (Exception e) {
      Log.e(TAG, e.getMessage());
    }
  }

  protected void checkPermissions(CallbackContext callbackContext) {
    boolean takePicturePermission = PermissionHelper.hasPermission(this, Manifest.permission.CAMERA);
    Log.i(TAG, "start - takePicturePermission: " + takePicturePermission);

    if (takePicturePermission) {
      PluginResult result = new PluginResult(PluginResult.Status.OK, "camera geen toegang");
      callbackContext.sendPluginResult(result);
    } else {
      PermissionHelper.requestPermission(this, 0, Manifest.permission.CAMERA);
      PluginResult result = new PluginResult(PluginResult.Status.ERROR, "camera geen toegang");
      callbackContext.sendPluginResult(result);
    }

  }

  /**
   * Send a success result to the webview
   * @param type
   * @param data
   */
  private void sendSuccessResult(final String type, final Object data) {
    final String currentResult = resultHashMap.get(type);
    final String newResult = String.valueOf(data);
    if (data == null || type == null || currentResult == newResult) {
      return;
    }
    cordova.getThreadPool().execute(new Runnable() {
      public void run() {
        JSONObject result = new JSONObject();
        try {
          result.put("resulttype", type);
          result.put("data", data);
          PluginResult pluginResult = new PluginResult(PluginResult.Status.OK, result);
          pluginResult.setKeepCallback(true);
          if (mainCallback != null) {
            mainCallback.sendPluginResult(pluginResult);
          } else {
            Log.d(TAG, "Queueing success result: " + pluginResult.getMessage());
            resultQueue.add(pluginResult);
          }
          resultHashMap.put(type, newResult);
        } catch (JSONException e) {
          Log.e(TAG, "could not serialize result for callback");
        }
      }
    });
  }

  /**
   * Send an error to the webview
   * @param type
   * @param message
   */
  private void sendErrorResult(final String type, final String message) {
    if (message != null && type != null) {
      cordova.getThreadPool().execute(new Runnable() {
        public void run() {
          JSONObject result = new JSONObject();
          try {
            result.put("resulttype", type);
            result.put("message", message);
            PluginResult pluginResult = new PluginResult(PluginResult.Status.ERROR, result);
            pluginResult.setKeepCallback(true);
            if (mainCallback != null) {
              Log.d(TAG, "Sending error result: " + pluginResult.getMessage());
              mainCallback.sendPluginResult(pluginResult);
            } else {
              Log.d(TAG, "Queueing error result: " + pluginResult.getMessage());
              resultQueue.add(pluginResult);
            }
          } catch (JSONException e) {
            Log.e(TAG, "could not serialize result for callback");
          }
        }
      });
    }
  }

  /**
   * Send results that were queued while the plugin was initializing
   */
  private void sendResultQueue() {
    for (PluginResult pluginResult : resultQueue) {
      mainCallback.sendPluginResult(pluginResult);
    }
    resultQueue.clear();
  }

  @Override
  public void onMeasurementCompleted(@NotNull HeartRate heartRate, @NotNull TimeHRV timeHRV, @NotNull FrequencyHRV frequencyHRV) {
    Log.d(TAG, "onHRVReady:" + String.valueOf(heartRate));
    JSONObject result = new JSONObject();
    try {
      result.put("avnn", timeHRV.getAVNN());
      result.put("bpm", heartRate.getBPM());
      result.put("sd", timeHRV.getSDNN());
      result.put("rmssd", timeHRV.getRMSSD());
      result.put("pnn50", timeHRV.getPNN50());
      result.put("confidenceLevel", heartRate.getConfidenceLevel());
      result.put("lfpercentage", frequencyHRV.getLowFrequencyPercentage());

      sendSuccessResult("hrv", result);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize HRV");
    }
  }

  @Override
  public void onHeartrateUpdated(HeartRate heartRate) {
    Log.d(TAG, "onHeartBeat:" + heartRate.getBPM());
    sendSuccessResult("bpm", (int) Math.round(heartRate.getBPM()));
  }

  @Override
  public void onFPSChanged(int fps) {
    Log.d(TAG, "onFPSChange:" + String.valueOf(fps));
    sendSuccessResult("fps", fps);
  }

  @Override
  public void onError(Error e) {
    Log.e(TAG, "onError:" + String.valueOf(e));
    String error = "";
    if (e == Error.NoFingerDetected) {
      error = "NO_FINGER_DETECTED";
    } else if (e == Error.CameraHasNoPermission) {
      error = "CAMERA_HAS_NO_PERMISSION";
    } else if (e == Error.FlashHasNoPermission) {
      error = "FLASH_HAS_NO_PERMISSION";
    } else if (e == Error.InvalidLicense) {
      error = "BAD_PACKAGE_NAME";
    } else if (e == Error.AccelerometerHasNoPermission) {
      error = "ACCELEROMETER_HAS_NO_PERMISSION";
    } else if (e == Error.LowBattery) {
      error = "LOW_BATTERY";
    } else if (e == Error.BadQuality) {
      error = "BAD_QUALITY";
    } else if (e == Error.TooMuchMovement) {
      error = "TOO_MUCH_MOVEMENT";
    }
    sendErrorResult("error", error);
  }

  @Override
  public void onWarning(Warning w) {
    Log.e(TAG, "onWarning:" + String.valueOf(w));
    String warning = "";
    if (w == Warning.MovementDetected) {
      warning = "SHAKING";
    } else if (w == Warning.LowQuality) {
      warning = "LOW_QUALITY";
    } else if (w == Warning.LowFps) {
      warning = "LOW_FPS";
    }

    sendSuccessResult("warning", warning);
  };


  @Override
  public void onStatusChanged(Status s) {
    Log.d(TAG, "onStatusChange:" + String.valueOf(s));
    String status = "ERROR";
    if (s == Status.Started) {
      status = "STARTED";
    } else if (s == Status.FingerDetected) {
      status = "FINGER_DETECTED";
    } else if (s == Status.Calibrating) {
      status = "CALIBRATING";
    } else if (s == Status.Measuring) {
      status = "MEASURING";
    } else if (s == Status.Completed) {
      status = "COMPLETED";
    } else if (s == Status.Error) {
      status = "ERROR";
    } else if (s == Status.NoFinger) {
      status = "NO_FINGER";
    }
    if (status == "ERROR" || status == "COMPLETED") {
      allowScreenToSleep();
    }
    sendSuccessResult("status", status);
  }


  @Override
  public void onGraphImageUpdated(Bitmap bitmap) {  }

  @Override
  public void onGraphDataUpdated(double[] ppgData) {
    if (ppgData.length == 0) {
      return;
    }
    Log.d(TAG, "onPPGDataUpdate:" + Arrays.toString(ppgData));
    try {
      JSONArray json = new JSONArray(ppgData);
      sendSuccessResult("graph", json);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize PPGData");
    }
  }

  @Override
  public void onProgressUpdated(int percentageCompleted, int timeProgressed, int timeRemaining) {
    Log.d(TAG, "onProgressUpdated:" + percentageCompleted + " timeProgressed: " + timeProgressed + " timeRemaining: " + timeRemaining);
    JSONObject result = new JSONObject();
    try {
      result.put("percentageCompleted", percentageCompleted);
      result.put("timeProgressed", timeProgressed);
      result.put("timeRemaining", timeRemaining);

      sendSuccessResult("progress", result);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize progress update");
    }
  }

  @Override
  public void onTimeHRVUpdated(@NotNull TimeHRV timeHRV) {}

  @Override
  public void onFrequencyHRVUpdated(@NotNull FrequencyHRV frequencyHRV) {
    Log.d(TAG, "onFrequencyHRVUpdated:" + String.valueOf(frequencyHRV));
    JSONObject result = new JSONObject();
    try {
      result.put("lfPercentage", frequencyHRV.getLowFrequencyPercentage());
      result.put("confidenceLevel", frequencyHRV.getConfidenceLevel());

      sendSuccessResult("lfpercentage", result);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize frequency HRV update");
    }
  }

}