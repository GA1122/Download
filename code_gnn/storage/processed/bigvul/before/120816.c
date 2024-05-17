void BluetoothDeviceChromeOS::OnConnectError(
    bool after_pairing,
    const ConnectErrorCallback& error_callback,
    const std::string& error_name,
    const std::string& error_message) {
  if (--num_connecting_calls_ == 0)
    adapter_->NotifyDeviceChanged(this);

  DCHECK(num_connecting_calls_ >= 0);
  LOG(WARNING) << object_path_.value() << ": Failed to connect device: "
               << error_name << ": " << error_message;
  VLOG(1) << object_path_.value() << ": " << num_connecting_calls_
          << " still in progress";

  ConnectErrorCode error_code = ERROR_UNKNOWN;
  if (error_name == bluetooth_device::kErrorFailed) {
    error_code = ERROR_FAILED;
  } else if (error_name == bluetooth_device::kErrorInProgress) {
    error_code = ERROR_INPROGRESS;
  } else if (error_name == bluetooth_device::kErrorNotSupported) {
    error_code = ERROR_UNSUPPORTED_DEVICE;
  }

  if (after_pairing)
    RecordPairingResult(error_code);
   error_callback.Run(error_code);
 }
