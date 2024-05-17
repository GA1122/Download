void UsbOpenDeviceFunction::OnDeviceOpened(
    scoped_refptr<UsbDeviceHandle> device_handle) {
  if (!device_handle.get()) {
    Respond(Error(kErrorOpen));
    return;
  }

  RecordDeviceLastUsed();

  ApiResourceManager<UsbDeviceResource>* manager =
      ApiResourceManager<UsbDeviceResource>::Get(browser_context());
  scoped_refptr<UsbDevice> device = device_handle->GetDevice();
  Respond(OneArgument(PopulateConnectionHandle(
      manager->Add(new UsbDeviceResource(extension_id(), device_handle)),
      device->vendor_id(), device->product_id())));
}
