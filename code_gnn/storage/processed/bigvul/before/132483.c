ExtensionFunction::ResponseAction UsbReleaseInterfaceFunction::Run() {
  scoped_ptr<extensions::core_api::usb::ReleaseInterface::Params> parameters =
      ReleaseInterface::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  if (device_handle->ReleaseInterface(parameters->interface_number)) {
    return RespondNow(NoArguments());
  } else {
    return RespondNow(Error(kErrorCannotReleaseInterface));
  }
}
