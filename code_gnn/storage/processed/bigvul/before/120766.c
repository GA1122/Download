void BluetoothAdapterChromeOS::DeviceRemoved(
    const dbus::ObjectPath& object_path) {
  for (DevicesMap::iterator iter = devices_.begin();
       iter != devices_.end(); ++iter) {
    BluetoothDeviceChromeOS* device_chromeos =
        static_cast<BluetoothDeviceChromeOS*>(iter->second);
    if (device_chromeos->object_path() == object_path) {
      devices_.erase(iter);

      FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                        DeviceRemoved(this, device_chromeos));
      delete device_chromeos;
      return;
    }
  }
}
