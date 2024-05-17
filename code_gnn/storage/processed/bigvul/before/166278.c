void VideoCaptureManager::OnDeviceLaunchAborted() {
  EmitLogMessage("Launching device has been aborted.", 1);
  device_start_request_queue_.pop_front();
  ProcessDeviceStartRequestQueue();
}
