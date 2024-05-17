void UsbConnectionFunction::ReleaseDeviceHandle(
    const extensions::core_api::usb::ConnectionHandle& handle) {
  ApiResourceManager<UsbDeviceResource>* manager =
      ApiResourceManager<UsbDeviceResource>::Get(browser_context());
  manager->Remove(extension_id(), handle.handle);
}
