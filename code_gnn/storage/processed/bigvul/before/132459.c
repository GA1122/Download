bool UsbPermissionCheckingFunction::HasDevicePermission(
    scoped_refptr<UsbDevice> device) {
  if (!device_permissions_manager_) {
    device_permissions_manager_ =
        DevicePermissionsManager::Get(browser_context());
  }

  DevicePermissions* device_permissions =
      device_permissions_manager_->GetForExtension(extension_id());
  DCHECK(device_permissions);

  permission_entry_ = device_permissions->FindUsbDeviceEntry(device);
  if (permission_entry_.get()) {
    return true;
  }

  UsbDevicePermission::CheckParam param(
      device->vendor_id(),
      device->product_id(),
      UsbDevicePermissionData::UNSPECIFIED_INTERFACE);
  if (extension()->permissions_data()->CheckAPIPermissionWithParam(
          APIPermission::kUsbDevice, &param)) {
    return true;
  }

  return false;
}
