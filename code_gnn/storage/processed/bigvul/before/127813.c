void PepperMediaDeviceManager::OnDevicesEnumerated(
    int request_id,
    const StreamDeviceInfoArray& device_array) {
  EnumerateCallbackMap::iterator iter = enumerate_callbacks_.find(request_id);
  if (iter == enumerate_callbacks_.end()) {
    return;
  }

  EnumerateDevicesCallback callback = iter->second;

  std::vector<ppapi::DeviceRefData> devices;
  devices.reserve(device_array.size());
  for (StreamDeviceInfoArray::const_iterator info = device_array.begin();
       info != device_array.end();
       ++info) {
    devices.push_back(FromStreamDeviceInfo(*info));
  }
  callback.Run(request_id, devices);
}
