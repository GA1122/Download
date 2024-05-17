void MediaStreamManager::TranslateDeviceIdToSourceId(
    DeviceRequest* request,
    MediaStreamDevice* device) {
  if (request->audio_type() == MEDIA_DEVICE_AUDIO_CAPTURE ||
      request->video_type() == MEDIA_DEVICE_VIDEO_CAPTURE) {
    device->id =
        GetHMACForMediaDeviceID(request->salt_and_origin.device_id_salt,
                                request->salt_and_origin.origin, device->id);
    if (device->group_id) {
      device->group_id = GetHMACForMediaDeviceID(
          request->salt_and_origin.group_id_salt,
          request->salt_and_origin.origin, *device->group_id);
    }
  }
}
