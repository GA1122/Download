void BluetoothAdapterChromeOS::AdapterAdded(
    const dbus::ObjectPath& object_path) {
  if (!IsPresent())
    SetAdapter(object_path);
}
