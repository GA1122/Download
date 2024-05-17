bool WebContext::setDefaultAudioCaptureDeviceId(const QString& id) {
  if (IsInitialized()) {
    return MediaCaptureDevicesContext::Get(context_.get())
        ->SetDefaultAudioDeviceId(id.toStdString());
  }

  construct_props_->default_audio_capture_device_id = id.toStdString();
  client_->DefaultAudioCaptureDeviceChanged();
  return true;
}
