QString WebContext::defaultVideoCaptureDeviceId() const {
  if (IsInitialized()) {
    return QString::fromStdString(
        MediaCaptureDevicesContext::Get(context_.get())
          ->GetDefaultVideoDeviceId());
  }

  return QString::fromStdString(
      construct_props_->default_video_capture_device_id);
}
