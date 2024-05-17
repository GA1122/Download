ExtensionFunction::ResponseAction UsbClaimInterfaceFunction::Run() {
  scoped_ptr<extensions::core_api::usb::ClaimInterface::Params> parameters =
      ClaimInterface::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  device_handle->ClaimInterface(
      parameters->interface_number,
      base::Bind(&UsbClaimInterfaceFunction::OnComplete, this));
  return RespondLater();
}
