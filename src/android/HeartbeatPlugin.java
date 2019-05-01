package nl.trimbos.eva;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Arrays;

import android.util.Log;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.SystemClock;
import android.view.WindowManager;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.PluginResult;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import happitech.nl.heartbeatandroid.Interface.HeartBeatListener;
import happitech.nl.heartbeatandroid.Monitor;
import happitech.nl.heartbeatandroid.models.ClassifiedResult;
import happitech.nl.heartbeatandroid.models.HRV;
import happitech.nl.heartbeatandroid.models.HR;
import happitech.nl.heartbeatandroid.models.Pulse;

public class HeartbeatPlugin extends CordovaPlugin implements HeartBeatListener {

  private static final String TAG = "HeartbeatPlugin";
  private Monitor monitor;
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
    monitor = new Monitor(cordova.getActivity(), this, measureTime);
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
    } else if (action.equals("getBatteryLevel")) {
      getBatteryLevel(callbackContext);
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
    PluginResult result = new PluginResult(PluginResult.Status.NO_RESULT, "");
    result.setKeepCallback(true);
    callbackContext.sendPluginResult(result);
    monitor.startMeasuring();

    keepScreenAwake();
  }

  /**
   * Stop the plugin
   * @param callbackContext
   */
  protected void stop(CallbackContext callbackContext) {
    Log.i(TAG, "stop");
    PluginResult result = new PluginResult(PluginResult.Status.NO_RESULT, "");
    callbackContext.sendPluginResult(result);
    monitor.stopMeasuring();

    allowScreenToSleep();
  }

  /**
   * Sets the measure time
   * @param args
   * @param callbackContext
   */
  protected void setMeasureTime(JSONArray args, CallbackContext callbackContext) {
    try {
      measureTime = args.getInt(0);
      monitor = new Monitor(cordova.getActivity(), this, measureTime);
      PluginResult result = new PluginResult(PluginResult.Status.NO_RESULT, "");
      callbackContext.sendPluginResult(result);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize result for callback");
    }
    Log.d(TAG, "Set measure time: " + measureTime);
  }

  /**
   * Gets the battery level
   * @param args
   * @param callbackContext
   */
  protected void getBatteryLevel(CallbackContext callbackContext) {
    float batteryLevel = monitor.getBatteryLevel();
    Log.d(TAG, "batteryLevel:" + String.valueOf(batteryLevel));
    sendSuccessResult("batteryLevel", batteryLevel);
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

  // private void sendDelayedSuccessResult(final String type, final Object data) {
  //   if (isSendingResult) {
  //     return;
  //   }
  //   isSendingResult = true;
  //   resultHandler.postDelayed(new Runnable() {
  //     @Override
  //     public void run() {
  //       sendSuccessResult(type, data);
  //       isSendingResult = false;
  //     }
  //   }, 1000);
  // }

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
  public void onHRVReady(HRV r, ClassifiedResult cr) {
    Log.d(TAG, "onHRVReady:" + String.valueOf(r));
    JSONObject result = new JSONObject();
    try {
      result.put("avnn", r.getAVNN());
      result.put("bpm", r.getBPM());
      result.put("sd", r.getSD());
      result.put("rmssd", r.getrMSSD());
      result.put("pnn50", r.getpNN50());
      result.put("confidenceLevel", r.getConfidenceLevel());
      // result.put("badBeats", r.getBadBeats());
      // result.put("totalBeats", r.getTotalBeats());
      sendSuccessResult("hrv", result);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize HRV");
    }
  }

  @Override
  public void onHeartBeat(int b) {
    Log.d(TAG, "onHeartBeat:" + String.valueOf(b));
    sendSuccessResult("bpm", b);
  }

  @Override
  public void onHeartBeat(HR hr) {
    Log.d(TAG, "onHeartBeatHr:" + String.valueOf(hr));
    JSONObject result = new JSONObject();
    try {
      result.put("timestamp", hr.getTimestamp());
      result.put("bpm", hr.getBPM());
      // result.put("corrolation", hr.getCorrolation());
      // result.put("confidenceLevel", hr.getConfidenceLevel());
      sendSuccessResult("hr", result);
    } catch (JSONException e) {
      Log.e(TAG, "could not serialize hr");
    }
  }

  @Override
  public void onFPSChange(long fps) {
    Log.d(TAG, "onFPSChange:" + String.valueOf(fps));
    sendSuccessResult("fps", fps);
  }

  @Override
  public void onError(Monitor.ERROR e) {
    Log.e(TAG, "onError:" + String.valueOf(e));
    String error = "";
    if (e == Monitor.ERROR.NO_FINGER_DETECTED) {
      error = "NO_FINGER_DETECTED";
    } else if (e == Monitor.ERROR.CAMERA_HAS_NO_PERMISSION) {
      error = "CAMERA_HAS_NO_PERMISSION";
    } else if (e == Monitor.ERROR.FLASH_HAS_NO_PERMISSION) {
      error = "FLASH_HAS_NO_PERMISSION";
    } else if (e == Monitor.ERROR.BAD_PACKAGE_NAME) {
      error = "BAD_PACKAGE_NAME";
    } else if (e == Monitor.ERROR.FPS_TO_LOW) {
      error = "FPS_TOO_LOW";
    } else if (e == Monitor.ERROR.ACCELEROMETER_HAS_NO_PERMISSION) {
      error = "ACCELEROMETER_HAS_NO_PERMISSION";
    } else if (e == Monitor.ERROR.LOW_BATTERY) {
      error = "LOW_BATTERY";
    }
    sendErrorResult("error", error);
  }

    @Override
    public void onWarning(Monitor.WARNING w) {
        String status = "WARNING";
        sendSuccessResult("status", status);
    };


  @Override
  public void onStatusChange(Monitor.STATUS s) {
    Log.d(TAG, "onStatusChange:" + String.valueOf(s));
    String status = "ERROR";
    if (s == Monitor.STATUS.STARTED) {
      status = "STARTED";
    } else if (s == Monitor.STATUS.STOPPED) {
      status = "STOPPED";
    } else if (s == Monitor.STATUS.FINGER_DETECTED) {
      status = "FINGER_DETECTED";
    } else if (s == Monitor.STATUS.CALIBRATING) {
      status = "CALIBRATING";
    } else if (s == Monitor.STATUS.MEASURING) {
      status = "MEASURING";
    } else if (s == Monitor.STATUS.COMPLETED) {
      status = "COMPLETED";
    } else if (s == Monitor.STATUS.ERROR) {
      status = "ERROR";
    } else if (s == Monitor.STATUS.NO_FINGER) {
      status = "NO_FINGER";
    } else if (s == Monitor.STATUS.INACCURATE_MEASUREMENT) {
      status = "INACCURATE_MEASUREMENT";
    } else if (s == Monitor.STATUS.LOW_RED_VALUE) {
      status = "LOW_RED_VALUE";
    }
    if (status == "ERROR" || status == "COMPLETED") {
      allowScreenToSleep();
    }
    sendSuccessResult("status", status);
  }

  @Override
  public void onBeatSignalStrength(double signal) {
    Log.d(TAG, "onBeatSignalStrength:" + String.valueOf(signal));
    // if (signal > 0) {
    //   sendSuccessResult("signal", signal);
    // }
  }

  @Override
  public void onPPGCanvasUpdate(Bitmap bitmap) {  }

  @Override
  public void onPPGDataUpdate(double[] ppgData) {
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
  public void onPercentageCompleted(int percentage) {
    sendSuccessResult("progress", percentage);
  }

}
