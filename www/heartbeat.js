/**
 * Heartbeat Cordova Plugin
 *
 * @author Wilco Pots
 */
var channel = require('cordova/channel'),
  exec = require('cordova/exec');

/**
 * Event emitter class
 * @returns {EventEmitter}
 */
function EventEmitter() {
  this._listeners = {}
  this._mutators = {}
}

EventEmitter.prototype.on = function(event, fn) {
  (this._listeners[event] = this._listeners[event] || []).push(fn);
  return this;
};

EventEmitter.prototype.mutate = function(event, fn) {
  this._mutators[event] = fn;
  return this;
};

EventEmitter.prototype.once = function(event, fn) {
  var self = this;
  return self.on(event, listener);

  function listener() {
    fn.apply(this, [].slice.call(arguments));
    self.remove(event, listener);
  }
};

EventEmitter.prototype.emit = function(event) {
  var list = this._listeners[event] || [],
    args = [].slice.call(arguments, 1),
    mutator = this._mutators[event] || function() {
      return [].slice.call(arguments);
    };

  args = mutator.apply(null, args);

  list.forEach(function(fn) {
    fn.apply(null, args);
  });
};

EventEmitter.prototype.listeners = function(event) {
  return this._listeners[event] || [];
};

EventEmitter.prototype.removeListener = EventEmitter.prototype.remove = function(event, fn) {
  var tmp = this._listeners[event] || [];
  typeof fn === 'function' ? tmp.indexOf(fn) > -1 && tmp.splice(tmp.indexOf(fn), 1) : delete this._listeners[event];
};

function inherit(proto) {
  function F() {}
  F.prototype = proto;
  return new F();
}

/**
 * Heartbeat Cordova plugin
 * @base {EventEmitter}
 * @type {Heartbeat}
 *
 * A singleton instance is automatically created by adding the plugin to your project
 * This singleton can be accessed as global variable `Heartbeat` in JavaScript
 */
function Heartbeat(){
  EventEmitter.apply(this, arguments);
}

Heartbeat.prototype = inherit(EventEmitter.prototype);

Heartbeat.prototype.start = function(success, fail){
  exec(this.successCallback.bind(this), this.errorCallback.bind(this), 'Heartbeat', 'start', []);
};

Heartbeat.prototype.stop = function(success, fail){
  exec(success, fail, 'Heartbeat', 'stop', []);
};

Heartbeat.prototype.setPointsForGraph = function(points, success, fail) {
  exec(success, fail, 'Heartbeat', 'setPointsForGraph', [points]);
};

Heartbeat.prototype.setMeasureTime = function(measureTime, success, fail) {
  exec(success, fail, 'Heartbeat', 'setMeasureTime', [measureTime]);
};

Heartbeat.prototype.getBatteryLevel = function(success, fail) {
  exec(success, fail, 'Heartbeat', 'getBatteryLevel', []);
};

Heartbeat.prototype.successCallback = function(payload) {
  console.log('successCallBack received', JSON.stringify(payload));
  if (payload && payload.type) {
    this.emit(payload.type, payload.data);
    console.log('Heartbeat.prototype.successCallback', payload);
    cordova.fireWindowEvent('successevent', payload);
  }
};

Heartbeat.prototype.errorCallback = function(payload) {
  console.log('errorCallback received', JSON.stringify(payload));
  if (payload && payload.type) {
    this.emit(payload.type, new Error(payload.message));
    console.log('Heartbeat.prototype.errorCallback', payload);
    cordova.fireWindowEvent('errorevent', payload);
  }
};

module.exports = new Heartbeat();
