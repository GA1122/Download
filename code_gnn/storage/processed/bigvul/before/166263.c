media::VideoCaptureDeviceInfo* VideoCaptureManager::GetDeviceInfoById(
    const std::string& id) {
  for (auto& it : devices_info_cache_) {
    if (it.descriptor.device_id == id)
      return &it;
  }
  return nullptr;
}
