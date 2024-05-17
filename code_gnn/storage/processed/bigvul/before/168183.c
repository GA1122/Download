bool UsbTabHelper::IsDeviceConnected() const {
  for (const auto& map_entry : frame_usb_services_) {
    if (map_entry.second->device_connection_count_ > 0)
      return true;
  }
  return false;
}
