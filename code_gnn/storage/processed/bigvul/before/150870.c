void BluetoothAdapter::NotifyGattServiceChanged(
    BluetoothRemoteGattService* service) {
  DCHECK_EQ(service->GetDevice()->GetAdapter(), this);

  for (auto& observer : observers_)
    observer.GattServiceChanged(this, service);
}
