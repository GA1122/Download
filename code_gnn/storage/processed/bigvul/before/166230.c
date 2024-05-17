bool MediaStreamManager::SetUpDeviceCaptureRequest(
    DeviceRequest* request,
    const MediaDeviceEnumeration& enumeration) {
  DCHECK((request->audio_type() == MEDIA_DEVICE_AUDIO_CAPTURE ||
          request->audio_type() == MEDIA_NO_SERVICE) &&
         (request->video_type() == MEDIA_DEVICE_VIDEO_CAPTURE ||
          request->video_type() == MEDIA_NO_SERVICE));
  std::string audio_device_id;
  if (request->controls.audio.requested &&
      !GetRequestedDeviceCaptureId(request, request->audio_type(),
                                   enumeration[MEDIA_DEVICE_TYPE_AUDIO_INPUT],
                                   &audio_device_id)) {
    return false;
  }

  std::string video_device_id;
  if (request->controls.video.requested &&
      !GetRequestedDeviceCaptureId(request, request->video_type(),
                                   enumeration[MEDIA_DEVICE_TYPE_VIDEO_INPUT],
                                   &video_device_id)) {
    return false;
  }
  request->CreateUIRequest(audio_device_id, video_device_id);
  DVLOG(3) << "Audio requested " << request->controls.audio.requested
           << " device id = " << audio_device_id << "Video requested "
           << request->controls.video.requested
           << " device id = " << video_device_id;
  return true;
}
