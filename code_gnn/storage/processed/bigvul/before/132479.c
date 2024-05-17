ExtensionFunction::ResponseAction UsbGetConfigurationFunction::Run() {
  scoped_ptr<extensions::core_api::usb::GetConfiguration::Params> parameters =
      GetConfiguration::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  const UsbConfigDescriptor* config_descriptor =
      device_handle->GetDevice()->GetConfiguration();
  if (config_descriptor) {
    ConfigDescriptor config;
    ConvertConfigDescriptor(*config_descriptor, &config);
    return RespondNow(OneArgument(config.ToValue().release()));
  } else {
    return RespondNow(Error(kErrorNotConfigured));
  }
}
