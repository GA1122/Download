ExtensionFunction::ResponseAction UsbCloseDeviceFunction::Run() {
  scoped_ptr<extensions::core_api::usb::CloseDevice::Params> parameters =
      CloseDevice::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  ReleaseDeviceHandle(parameters->handle);
  return RespondNow(NoArguments());
}
