QString WebContext::defaultAudioCaptureDeviceId() const {
  if (IsInitialized()) {
    return QString::fromStdString(
        MediaCaptureDevicesContext::Get(context_.get())
          ->GetDefaultAudioDeviceId());
  }

  return QString::fromStdString(
      construct_props_->default_audio_capture_device_id);
}
