UsbSetInterfaceAlternateSettingFunction::Run() {
  scoped_ptr<extensions::core_api::usb::SetInterfaceAlternateSetting::Params>
      parameters = SetInterfaceAlternateSetting::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  scoped_refptr<UsbDeviceHandle> device_handle =
      GetDeviceHandle(parameters->handle);
  if (!device_handle.get()) {
    return RespondNow(Error(kErrorNoConnection));
  }

  device_handle->SetInterfaceAlternateSetting(
      parameters->interface_number, parameters->alternate_setting,
      base::Bind(&UsbSetInterfaceAlternateSettingFunction::OnComplete, this));
  return RespondLater();
}
