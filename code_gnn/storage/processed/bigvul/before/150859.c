void BluetoothAdapter::NotifyDeviceChanged(BluetoothDevice* device) {
  DCHECK(device);
  DCHECK_EQ(device->GetAdapter(), this);

  for (auto& observer : observers_)
    observer.DeviceChanged(this, device);
}
