void BluetoothAdapterChromeOS::NotifyDeviceChanged(
    BluetoothDeviceChromeOS* device) {
  DCHECK(device->adapter_ == this);

  FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                    DeviceChanged(this, device));
}
