void PepperMediaDeviceManager::OnDeviceOpened(
    int request_id,
    const std::string& label,
    const StreamDeviceInfo& device_info) {
  NotifyDeviceOpened(request_id, true, label);
}
