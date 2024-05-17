void MediaStreamManager::TranslateDeviceIdToSourceId(
    DeviceRequest* request,
    MediaStreamDevice* device) {
  if (request->audio_type() == MEDIA_DEVICE_AUDIO_CAPTURE ||
      request->video_type() == MEDIA_DEVICE_VIDEO_CAPTURE) {
    device->id = GetHMACForMediaDeviceID(request->salt,
                                         request->security_origin, device->id);
  }
}
