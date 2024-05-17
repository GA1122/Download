void MediaStreamDevicesController::NotifyUIRequestDenied() const {
  if (!content_settings_)
    return;

  if (request_.audio_type == content::MEDIA_TAB_AUDIO_CAPTURE ||
      request_.video_type == content::MEDIA_TAB_VIDEO_CAPTURE) {
      return;
  }

  if (request_.audio_type == content::MEDIA_DEVICE_AUDIO_CAPTURE)
    content_settings_->OnMicrophoneAccessBlocked();
  if (request_.video_type == content::MEDIA_DEVICE_VIDEO_CAPTURE)
    content_settings_->OnCameraAccessBlocked();
}
