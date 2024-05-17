bool MediaStreamManager::PickDeviceId(
    const MediaDeviceSaltAndOrigin& salt_and_origin,
    const TrackControls& controls,
    const MediaDeviceInfoArray& devices,
    std::string* device_id) const {
  if (controls.device_id.empty())
    return true;

  if (!GetDeviceIDFromHMAC(salt_and_origin.device_id_salt,
                           salt_and_origin.origin, controls.device_id, devices,
                           device_id)) {
    LOG(WARNING) << "Invalid device ID = " << controls.device_id;
    return false;
  }
  return true;
}
