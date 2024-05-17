bool MediaStreamManager::PickDeviceId(const std::string& salt,
                                      const url::Origin& security_origin,
                                      const TrackControls& controls,
                                      const MediaDeviceInfoArray& devices,
                                      std::string* device_id) const {
  if (controls.device_id.empty())
    return true;

  if (!GetDeviceIDFromHMAC(salt, security_origin, controls.device_id, devices,
                           device_id)) {
    LOG(WARNING) << "Invalid device ID = " << controls.device_id;
    return false;
  }
  return true;
}
