  void RemoveMockDevice() {
    DCHECK(mock_device_);
    device_client_->usb_service()->RemoveDevice(mock_device_);
    mock_device_ = nullptr;
  }
