 void BluetoothAdapterChromeOS::SetAdapter(const dbus::ObjectPath& object_path) {
   DCHECK(!IsPresent());
   object_path_ = object_path;

  VLOG(1) << object_path_.value() << ": using adapter.";

  SetDefaultAdapterName();

  BluetoothAdapterClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothAdapterClient()->
          GetProperties(object_path_);

  PresentChanged(true);

  if (properties->powered.value())
    PoweredChanged(true);
  if (properties->discoverable.value())
    DiscoverableChanged(true);
  if (properties->discovering.value())
    DiscoveringChanged(true);

  std::vector<dbus::ObjectPath> device_paths =
      DBusThreadManager::Get()->GetBluetoothDeviceClient()->
          GetDevicesForAdapter(object_path_);

  for (std::vector<dbus::ObjectPath>::iterator iter = device_paths.begin();
       iter != device_paths.end(); ++iter) {
    BluetoothDeviceChromeOS* device_chromeos =
        new BluetoothDeviceChromeOS(this, *iter);

    devices_[device_chromeos->GetAddress()] = device_chromeos;

    FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                      DeviceAdded(this, device_chromeos));
  }
}
