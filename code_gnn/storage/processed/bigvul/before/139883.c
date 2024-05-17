void MidiManagerUsb::Initialize(
    base::Callback<void(MidiResult result)> callback) {
  initialize_callback_ = callback;
  device_factory_->EnumerateDevices(
      this,
      base::Bind(&MidiManagerUsb::OnEnumerateDevicesDone,
                 base::Unretained(this)));
}
