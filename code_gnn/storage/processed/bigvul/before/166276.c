void VideoCaptureManager::OnDeviceConnectionLost(
    VideoCaptureController* controller) {
  std::ostringstream string_stream;
  string_stream << "Lost connection to device. device_id = "
                << controller->device_id();
  EmitLogMessage(string_stream.str(), 1);
  controller->OnError(
      media::VideoCaptureError::kVideoCaptureManagerDeviceConnectionLost);
}
