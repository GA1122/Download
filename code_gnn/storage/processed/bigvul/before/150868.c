void BluetoothAdapter::NotifyGattDiscoveryComplete(
    BluetoothRemoteGattService* service) {
  DCHECK_EQ(service->GetDevice()->GetAdapter(), this);

  for (auto& observer : observers_)
    observer.GattDiscoveryCompleteForService(this, service);
}
