void MediaStreamManager::DevicesEnumerated(
    bool requested_audio_input,
    bool requested_video_input,
    const std::string& label,
    const MediaDeviceEnumeration& enumeration) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  DeviceRequest* request = FindRequest(label);
  if (!request)
    return;

  bool requested[] = {requested_audio_input, requested_video_input};
  MediaStreamType stream_types[] = {MEDIA_DEVICE_AUDIO_CAPTURE,
                                    MEDIA_DEVICE_VIDEO_CAPTURE};
  for (size_t i = 0; i < arraysize(requested); ++i) {
    if (!requested[i])
      continue;

    DCHECK(request->audio_type() == stream_types[i] ||
           request->video_type() == stream_types[i]);
    if (request->state(stream_types[i]) == MEDIA_REQUEST_STATE_REQUESTED) {
      request->SetState(stream_types[i], MEDIA_REQUEST_STATE_PENDING_APPROVAL);
    }
  }

  if (!SetupDeviceCaptureRequest(request, enumeration))
    FinalizeRequestFailed(label, request, MEDIA_DEVICE_NO_HARDWARE);
  else
    ReadOutputParamsAndPostRequestToUI(label, request, enumeration);
}
