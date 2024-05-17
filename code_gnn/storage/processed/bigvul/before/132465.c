void UsbFindDevicesFunction::OnDeviceOpened(
    scoped_refptr<UsbDeviceHandle> device_handle) {
  if (device_handle.get()) {
    ApiResourceManager<UsbDeviceResource>* manager =
        ApiResourceManager<UsbDeviceResource>::Get(browser_context());
    UsbDeviceResource* resource =
        new UsbDeviceResource(extension_id(), device_handle);
    scoped_refptr<UsbDevice> device = device_handle->GetDevice();
    result_->Append(PopulateConnectionHandle(
        manager->Add(resource), device->vendor_id(), device->product_id()));
  }
  barrier_.Run();
}
