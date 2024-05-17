void MediaStreamManager::SetupRequest(const std::string& label) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DeviceRequest* request = FindRequest(label);
  if (!request) {
    DVLOG(1) << "SetupRequest label " << label << " doesn't exist!!";
    return;   
  }

  MediaStreamType audio_type = MEDIA_NO_SERVICE;
  MediaStreamType video_type = MEDIA_NO_SERVICE;
  ParseStreamType(request->controls, &audio_type, &video_type);
  request->SetAudioType(audio_type);
  request->SetVideoType(video_type);

  const bool is_web_contents_capture = audio_type == MEDIA_TAB_AUDIO_CAPTURE ||
                                       video_type == MEDIA_TAB_VIDEO_CAPTURE;
  if (is_web_contents_capture && !SetupTabCaptureRequest(request)) {
    FinalizeRequestFailed(label,
                          request,
                          MEDIA_DEVICE_TAB_CAPTURE_FAILURE);
    return;
  }

  const bool is_screen_capture = video_type == MEDIA_DESKTOP_VIDEO_CAPTURE;
  if (is_screen_capture && !SetupScreenCaptureRequest(request)) {
    FinalizeRequestFailed(label,
                          request,
                          MEDIA_DEVICE_SCREEN_CAPTURE_FAILURE);
    return;
  }

  if (!is_web_contents_capture && !is_screen_capture) {
    if (audio_type == MEDIA_DEVICE_AUDIO_CAPTURE ||
        video_type == MEDIA_DEVICE_VIDEO_CAPTURE) {
      StartEnumeration(request, label);
      return;
    }
    if (!SetupDeviceCaptureRequest(request, MediaDeviceEnumeration())) {
      FinalizeRequestFailed(label, request, MEDIA_DEVICE_NO_HARDWARE);
      return;
    }
  }
  ReadOutputParamsAndPostRequestToUI(label, request, MediaDeviceEnumeration());
}
