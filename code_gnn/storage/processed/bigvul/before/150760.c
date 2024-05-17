void BluetoothDeviceChooserController::PostSuccessCallback(
    const std::string& device_address) {
  if (!base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::BindOnce(success_callback_, std::move(options_),
                                    device_address))) {
    LOG(WARNING) << "No TaskRunner.";
  }
 }
