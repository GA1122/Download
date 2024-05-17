ExtensionFunction::ResponseAction UsbListInterfacesFunction::Run() {
  scoped_ptr<extensions::core_api::usb::ListInterfaces::Params> parameters =
      ListInterfaces::Params::Create(*args_);
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

    scoped_ptr<base::ListValue> result(new base::ListValue);
    for (size_t i = 0; i < config.interfaces.size(); ++i) {
      result->Append(config.interfaces[i]->ToValue().release());
    }

    return RespondNow(OneArgument(result.release()));
  } else {
    return RespondNow(Error(kErrorNotConfigured));
  }
}
