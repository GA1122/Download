void BluetoothAdapter::NotifyGattDescriptorValueChanged(
    BluetoothRemoteGattDescriptor* descriptor,
    const std::vector<uint8_t>& value) {
  DCHECK_EQ(
      descriptor->GetCharacteristic()->GetService()->GetDevice()->GetAdapter(),
      this);

  for (auto& observer : observers_)
    observer.GattDescriptorValueChanged(this, descriptor, value);
}
