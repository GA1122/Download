void BluetoothAdapterChromeOS::RemoveAdapter() {
  DCHECK(IsPresent());
  VLOG(1) << object_path_.value() << ": adapter removed.";

  BluetoothAdapterClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothAdapterClient()->
          GetProperties(object_path_);

  object_path_ = dbus::ObjectPath("");

  if (properties->powered.value())
    PoweredChanged(false);
  if (properties->discoverable.value())
    DiscoverableChanged(false);
  if (properties->discovering.value())
    DiscoveringChanged(false);

  DevicesMap devices = devices_;
  devices_.clear();

  for (DevicesMap::iterator iter = devices.begin();
       iter != devices.end(); ++iter) {
    FOR_EACH_OBSERVER(BluetoothAdapter::Observer, observers_,
                      DeviceRemoved(this, iter->second));
    delete iter->second;
  }

  PresentChanged(false);
}
