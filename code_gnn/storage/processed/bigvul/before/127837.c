PepperPlatformAudioInput::~PepperPlatformAudioInput() {
  DCHECK(!ipc_);
  DCHECK(!client_);
  DCHECK(label_.empty());
  DCHECK(!pending_open_device_);
}
