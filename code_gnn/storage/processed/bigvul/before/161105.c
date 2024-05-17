MediaStreamDevices MediaStreamManager::ConvertToMediaStreamDevices(
    MediaStreamType stream_type,
    const MediaDeviceInfoArray& device_infos) {
  MediaStreamDevices devices;
  for (const auto& info : device_infos)
    devices.emplace_back(stream_type, info.device_id, info.label,
                         info.video_facing);

  if (stream_type != MEDIA_DEVICE_VIDEO_CAPTURE)
    return devices;

  for (auto& device : devices) {
    device.camera_calibration =
        video_capture_manager()->GetCameraCalibration(device.id);
  }
  return devices;
}
