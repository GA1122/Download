bool GetDeviceIDFromHMAC(const std::string& salt,
                         const url::Origin& security_origin,
                         const std::string& hmac_device_id,
                         const MediaDeviceInfoArray& devices,
                         std::string* device_id) {
  if (hmac_device_id.empty())
    return false;

  for (const auto& device_info : devices) {
    if (MediaStreamManager::DoesMediaDeviceIDMatchHMAC(
            salt, security_origin, hmac_device_id, device_info.device_id)) {
      *device_id = device_info.device_id;
      return true;
    }
  }
  return false;
}
