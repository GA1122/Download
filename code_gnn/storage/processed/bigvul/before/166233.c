bool MediaStreamManager::SetUpScreenCaptureRequest(DeviceRequest* request) {
  DCHECK(request->audio_type() == MEDIA_GUM_DESKTOP_AUDIO_CAPTURE ||
         request->video_type() == MEDIA_GUM_DESKTOP_VIDEO_CAPTURE);

  if (request->video_type() != MEDIA_GUM_DESKTOP_VIDEO_CAPTURE ||
      (request->audio_type() != MEDIA_NO_SERVICE &&
       request->audio_type() != MEDIA_GUM_DESKTOP_AUDIO_CAPTURE)) {
    LOG(ERROR) << "Invalid screen capture request.";
    return false;
  }

  std::string video_device_id;
  if (request->video_type() == MEDIA_GUM_DESKTOP_VIDEO_CAPTURE &&
      !request->controls.video.device_id.empty()) {
    video_device_id = request->controls.video.device_id;
  }

  const std::string audio_device_id =
      request->audio_type() == MEDIA_GUM_DESKTOP_AUDIO_CAPTURE ? video_device_id
                                                               : "";

  request->CreateUIRequest(audio_device_id, video_device_id);
  return true;
}
