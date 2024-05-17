int MediaStreamManager::VideoDeviceIdToSessionId(
    const std::string& device_id) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (const LabeledDeviceRequest& device_request : requests_) {
    for (const MediaStreamDevice& device : device_request.second->devices) {
      if (device.id == device_id && device.type == MEDIA_DEVICE_VIDEO_CAPTURE) {
        return device.session_id;
      }
    }
  }
  return MediaStreamDevice::kNoId;
}
