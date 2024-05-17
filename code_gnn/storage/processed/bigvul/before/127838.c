void PepperPlatformVideoCapture::DetachEventHandler() {
  handler_ = NULL;
  StopCapture();
  if (!release_device_cb_.is_null()) {
    release_device_cb_.Run();
    release_device_cb_.Reset();
  }
  if (!label_.empty()) {
    PepperMediaDeviceManager* const device_manager = GetMediaDeviceManager();
    if (device_manager)
      device_manager->CloseDevice(label_);
    label_.clear();
  }
  if (pending_open_device_) {
    PepperMediaDeviceManager* const device_manager = GetMediaDeviceManager();
    if (device_manager)
      device_manager->CancelOpenDevice(pending_open_device_id_);
    pending_open_device_ = false;
    pending_open_device_id_ = -1;
  }
}
