  void AddMockDevice(const std::string& serial_number) {
    DCHECK(!mock_device_);
    mock_device_ = base::MakeRefCounted<MockUsbDevice>(
        0, 0, "Test Manufacturer", "Test Device", serial_number);
    device_client_->usb_service()->AddDevice(mock_device_);
  }
