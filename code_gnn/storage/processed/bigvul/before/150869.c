void BluetoothAdapter::NotifyGattServiceAdded(
    BluetoothRemoteGattService* service) {
  DCHECK_EQ(service->GetDevice()->GetAdapter(), this);

  for (auto& observer : observers_)
    observer.GattServiceAdded(this, service->GetDevice(), service);
}
