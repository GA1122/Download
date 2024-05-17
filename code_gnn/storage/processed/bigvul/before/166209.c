bool MediaStreamManager::GetRequestedDeviceCaptureId(
    const DeviceRequest* request,
    MediaStreamType type,
    const MediaDeviceInfoArray& devices,
    std::string* device_id) const {
  if (type == MEDIA_DEVICE_AUDIO_CAPTURE) {
    return PickDeviceId(request->salt_and_origin, request->controls.audio,
                        devices, device_id);
  } else if (type == MEDIA_DEVICE_VIDEO_CAPTURE) {
    return PickDeviceId(request->salt_and_origin, request->controls.video,
                        devices, device_id);
  } else {
    NOTREACHED();
  }
  return false;
}
