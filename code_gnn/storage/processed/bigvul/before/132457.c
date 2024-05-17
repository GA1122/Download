scoped_refptr<device::UsbDeviceHandle> UsbConnectionFunction::GetDeviceHandle(
    const extensions::core_api::usb::ConnectionHandle& handle) {
  ApiResourceManager<UsbDeviceResource>* manager =
      ApiResourceManager<UsbDeviceResource>::Get(browser_context());
  if (!manager) {
    return nullptr;
  }

  UsbDeviceResource* resource = manager->Get(extension_id(), handle.handle);
  if (!resource) {
    return nullptr;
  }

  return resource->device();
}
