void PepperPlatformAudioInput::OnDeviceOpened(int request_id,
                                              bool succeeded,
                                              const std::string& label) {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

  pending_open_device_ = false;
  pending_open_device_id_ = -1;

  PepperMediaDeviceManager* const device_manager = GetMediaDeviceManager();
  if (succeeded && device_manager) {
    DCHECK(!label.empty());
    label_ = label;

    if (client_) {
      int session_id = device_manager->GetSessionID(
          PP_DEVICETYPE_DEV_AUDIOCAPTURE, label);
      io_message_loop_proxy_->PostTask(
          FROM_HERE,
          base::Bind(&PepperPlatformAudioInput::InitializeOnIOThread,
                     this,
                     session_id));
    } else {
      CloseDevice();
    }
  } else {
    NotifyStreamCreationFailed();
  }
}
