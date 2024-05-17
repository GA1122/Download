void PepperMediaDeviceManager::NotifyDeviceOpened(int request_id,
                                                  bool succeeded,
                                                  const std::string& label) {
  OpenCallbackMap::iterator iter = open_callbacks_.find(request_id);
  if (iter == open_callbacks_.end()) {
    return;
  }

  OpenDeviceCallback callback = iter->second;
  open_callbacks_.erase(iter);

  callback.Run(request_id, succeeded, label);
}
