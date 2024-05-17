bool WebContext::setDefaultVideoCaptureDeviceId(const QString& id) {
  if (IsInitialized()) {
    return MediaCaptureDevicesContext::Get(context_.get())
        ->SetDefaultVideoDeviceId(id.toStdString());
  }

  construct_props_->default_video_capture_device_id = id.toStdString();
  client_->DefaultVideoCaptureDeviceChanged();
  return true;
}
