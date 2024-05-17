ExtensionFunction::ResponseAction UsbSetConfigurationFunction::Run() {
  scoped_ptr<extensions::core_api::usb::SetConfiguration::Params> parameters =
      SetConfiguration::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  device_handle->SetConfiguration(
      parameters->configuration_value,
      base::Bind(&UsbSetConfigurationFunction::OnComplete, this));
  return RespondLater();
}
