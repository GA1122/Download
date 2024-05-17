void BluetoothAdapterChromeOS::SetDiscoverable(
    bool discoverable,
    const base::Closure& callback,
    const ErrorCallback& error_callback) {
  DBusThreadManager::Get()->GetBluetoothAdapterClient()->
      GetProperties(object_path_)->discoverable.Set(
          discoverable,
          base::Bind(&BluetoothAdapterChromeOS::OnSetDiscoverable,
                     weak_ptr_factory_.GetWeakPtr(),
                     callback,
                     error_callback));
}
