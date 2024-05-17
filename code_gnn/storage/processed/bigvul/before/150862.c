void BluetoothAdapter::NotifyGattCharacteristicAdded(
    BluetoothRemoteGattCharacteristic* characteristic) {
  DCHECK_EQ(characteristic->GetService()->GetDevice()->GetAdapter(), this);

  for (auto& observer : observers_)
    observer.GattCharacteristicAdded(this, characteristic);
}
