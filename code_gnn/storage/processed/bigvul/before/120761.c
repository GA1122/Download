void BluetoothAdapterChromeOS::AdapterPropertyChanged(
    const dbus::ObjectPath& object_path,
    const std::string& property_name) {
  if (object_path != object_path_)
    return;

  BluetoothAdapterClient::Properties* properties =
      DBusThreadManager::Get()->GetBluetoothAdapterClient()->
          GetProperties(object_path_);

  if (property_name == properties->powered.name())
    PoweredChanged(properties->powered.value());
  else if (property_name == properties->discoverable.name())
    DiscoverableChanged(properties->discoverable.value());
  else if (property_name == properties->discovering.name())
    DiscoveringChanged(properties->discovering.value());
}
