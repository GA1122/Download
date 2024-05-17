void BluetoothAdapter::NotifyGattDescriptorAdded(
    BluetoothRemoteGattDescriptor* descriptor) {
  DCHECK_EQ(
      descriptor->GetCharacteristic()->GetService()->GetDevice()->GetAdapter(),
      this);

  for (auto& observer : observers_)
    observer.GattDescriptorAdded(this, descriptor);
}
