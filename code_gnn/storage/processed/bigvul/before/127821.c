void PepperPlatformAudioInput::CloseDevice() {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

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
