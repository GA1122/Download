ppapi::DeviceRefData FromStreamDeviceInfo(const StreamDeviceInfo& info) {
  ppapi::DeviceRefData data;
  data.id = info.device.id;
  data.name = info.device.name.empty() ? std::string(" ") : info.device.name;
  data.type = PepperMediaDeviceManager::FromMediaStreamType(info.device.type);
  return data;
}
