MediaStreamDevices MediaStreamManager::GetDevicesOpenedByRequest(
    const std::string& label) const {
  DeviceRequest* request = FindRequest(label);
  if (!request)
    return MediaStreamDevices();
  return request->devices;
}
