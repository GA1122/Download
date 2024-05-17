ExtensionFunction::ResponseAction UsbOpenDeviceFunction::Run() {
  scoped_ptr<extensions::core_api::usb::OpenDevice::Params> parameters =
      OpenDevice::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  UsbService* service = device::DeviceClient::Get()->GetUsbService();
  if (!service) {
    return RespondNow(Error(kErrorInitService));
  }

  std::string guid;
  if (!UsbGuidMap::Get(browser_context())
           ->GetGuidFromId(parameters->device.device, &guid)) {
    return RespondNow(Error(kErrorNoDevice));
  }

  scoped_refptr<UsbDevice> device = service->GetDevice(guid);
  if (!device.get()) {
    return RespondNow(Error(kErrorNoDevice));
  }

  if (!HasDevicePermission(device)) {
    return RespondNow(Error(kErrorNoDevice));
  }

  device->Open(base::Bind(&UsbOpenDeviceFunction::OnDeviceOpened, this));
  return RespondLater();
}
