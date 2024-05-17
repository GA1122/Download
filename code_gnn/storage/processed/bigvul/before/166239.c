void MediaStreamManager::StopRemovedDevice(
    MediaDeviceType type,
    const MediaDeviceInfo& media_device_info) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(type == MEDIA_DEVICE_TYPE_AUDIO_INPUT ||
         type == MEDIA_DEVICE_TYPE_VIDEO_INPUT);

  MediaStreamType stream_type = ConvertToMediaStreamType(type);
  std::vector<int> session_ids;
  for (const LabeledDeviceRequest& labeled_request : requests_) {
    const DeviceRequest* request = labeled_request.second;
    for (const MediaStreamDevice& device : request->devices) {
      const std::string source_id = GetHMACForMediaDeviceID(
          request->salt_and_origin.device_id_salt,
          request->salt_and_origin.origin, media_device_info.device_id);
      if (device.id == source_id && device.type == stream_type) {
        session_ids.push_back(device.session_id);
        if (request->device_stopped_cb) {
          request->device_stopped_cb.Run(labeled_request.first, device);
        }
      }
    }
  }
  for (const int session_id : session_ids)
    StopDevice(stream_type, session_id);

  AddLogMessageOnIOThread(
      base::StringPrintf(
          "Media input device removed: type=%s, id=%s, name=%s ",
          (stream_type == MEDIA_DEVICE_AUDIO_CAPTURE ? "audio" : "video"),
          media_device_info.device_id.c_str(), media_device_info.label.c_str())
          .c_str());
}
