void VideoCaptureManager::OnDeviceLaunchFailed(
    VideoCaptureController* controller,
    media::VideoCaptureError error) {
  std::ostringstream string_stream;
  string_stream << "Launching device has failed. device_id = "
                << controller->device_id();
  EmitLogMessage(string_stream.str(), 1);
  controller->OnError(error);

  device_start_request_queue_.pop_front();
  ProcessDeviceStartRequestQueue();
}
