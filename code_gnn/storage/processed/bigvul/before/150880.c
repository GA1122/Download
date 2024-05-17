void BluetoothAdapter::RemoveTimedOutDevices() {
  for (auto it = devices_.begin(); it != devices_.end();) {
    BluetoothDevice* device = it->second.get();
    if (device->IsPaired() || device->IsConnected() ||
        device->IsGattConnected()) {
      ++it;
      continue;
    }

    base::Time last_update_time = device->GetLastUpdateTime();

    bool device_expired =
        (base::Time::NowFromSystemTime() - last_update_time) > timeoutSec;
    VLOG(3) << "device: " << device->GetAddress()
            << ", last_update: " << last_update_time
            << ", exp: " << device_expired;

    if (!device_expired) {
      ++it;
      continue;
    }

    VLOG(1) << "Removing device: " << device->GetAddress();
    auto next = it;
    next++;
    std::unique_ptr<BluetoothDevice> removed_device = std::move(it->second);
    devices_.erase(it);
    it = next;

    for (auto& observer : observers_)
      observer.DeviceRemoved(this, removed_device.get());
  }
}
