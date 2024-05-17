void PepperMediaDeviceManager::OnDeviceOpenFailed(int request_id) {
  NotifyDeviceOpened(request_id, false, std::string());
}
