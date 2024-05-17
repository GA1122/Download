void MediaStreamDevicesController::NotifyUIRequestAccepted() const {
  if (!content_settings_)
    return;

  if (request_.audio_type == content::MEDIA_DEVICE_AUDIO_CAPTURE) {
    if (microphone_requested_)
      content_settings_->OnMicrophoneAccessed();
    else
      content_settings_->OnMicrophoneAccessBlocked();
  }

  if (request_.video_type == content::MEDIA_DEVICE_VIDEO_CAPTURE) {
    if (webcam_requested_)
      content_settings_->OnCameraAccessed();
    else
      content_settings_->OnCameraAccessBlocked();
  }
}
