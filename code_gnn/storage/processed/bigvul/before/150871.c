void BluetoothAdapter::NotifyGattServiceRemoved(
    BluetoothRemoteGattService* service) {
  DCHECK_EQ(service->GetDevice()->GetAdapter(), this);

  for (auto& observer : observers_)
    observer.GattServiceRemoved(this, service->GetDevice(), service);
}
