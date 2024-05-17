void MediaStreamManager::CloseDevice(MediaStreamType type, int session_id) {
  DVLOG(1) << "CloseDevice("
           << "{type = " << type <<  "} "
           << "{session_id = " << session_id << "})";
  GetDeviceManager(type)->Close(session_id);

  for (const LabeledDeviceRequest& labeled_request : requests_) {
    DeviceRequest* const request = labeled_request.second;
    for (const MediaStreamDevice& device : request->devices) {
      if (device.session_id == session_id && device.type == type) {
        request->SetState(type, MEDIA_REQUEST_STATE_CLOSING);
      }
    }
  }
}
