void BluetoothAdapter::NotifyDevicePairedChanged(BluetoothDevice* device,
                                                 bool new_paired_status) {
  for (auto& observer : observers_)
    observer.DevicePairedChanged(this, device, new_paired_status);
}
