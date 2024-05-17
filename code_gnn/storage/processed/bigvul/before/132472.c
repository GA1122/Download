void UsbPermissionCheckingFunction::RecordDeviceLastUsed() {
  if (permission_entry_.get()) {
    device_permissions_manager_->UpdateLastUsed(extension_id(),
                                                permission_entry_);
  }
}
