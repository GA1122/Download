PepperPlatformVideoCapture::PepperPlatformVideoCapture(
    int render_frame_id,
    const std::string& device_id,
    const GURL& document_url,
    PepperVideoCaptureHost* handler)
    : render_frame_id_(render_frame_id),
      device_id_(device_id),
      session_id_(0),
      handler_(handler),
      pending_open_device_(false),
      pending_open_device_id_(-1),
      weak_factory_(this) {
  PepperMediaDeviceManager* const device_manager = GetMediaDeviceManager();
  if (device_manager) {
    pending_open_device_id_ = device_manager->OpenDevice(
        PP_DEVICETYPE_DEV_VIDEOCAPTURE,
        device_id,
        document_url,
        base::Bind(&PepperPlatformVideoCapture::OnDeviceOpened,
                   weak_factory_.GetWeakPtr()));
    pending_open_device_ = true;
  }
}
