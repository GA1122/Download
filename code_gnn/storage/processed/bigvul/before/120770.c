 BluetoothAdapterChromeOS::GetDeviceWithPath(
     const dbus::ObjectPath& object_path) {
  for (DevicesMap::iterator iter = devices_.begin();
       iter != devices_.end(); ++iter) {
    BluetoothDeviceChromeOS* device_chromeos =
        static_cast<BluetoothDeviceChromeOS*>(iter->second);
    if (device_chromeos->object_path() == object_path)
      return device_chromeos;
  }

   return NULL;
 }
