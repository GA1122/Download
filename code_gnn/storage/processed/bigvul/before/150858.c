void BluetoothAdapter::NotifyDeviceBatteryChanged(BluetoothDevice* device) {
  DCHECK_EQ(device->GetAdapter(), this);
  for (auto& observer : observers_) {
    observer.DeviceBatteryChanged(this, device, device->battery_percentage());
  }
}
