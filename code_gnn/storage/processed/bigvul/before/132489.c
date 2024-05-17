ExtensionFunction::ResponseAction UsbResetDeviceFunction::Run() {
  parameters_ = ResetDevice::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters_.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters_->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  device_handle->ResetDevice(
      base::Bind(&UsbResetDeviceFunction::OnComplete, this));
  return RespondLater();
}
