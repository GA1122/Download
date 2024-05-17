void BluetoothAdapter::NotifyGattCharacteristicValueChanged(
    BluetoothRemoteGattCharacteristic* characteristic,
    const std::vector<uint8_t>& value) {
  DCHECK_EQ(characteristic->GetService()->GetDevice()->GetAdapter(), this);

  base::WeakPtr<BluetoothRemoteGattCharacteristic> weak_characteristic =
      characteristic->GetWeakPtr();
  for (auto& observer : observers_) {
    if (!weak_characteristic)
      break;
    observer.GattCharacteristicValueChanged(this, characteristic, value);
  }
}
