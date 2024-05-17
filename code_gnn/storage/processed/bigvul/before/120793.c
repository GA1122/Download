void BluetoothAdapterChromeOS::SetPowered(
    bool powered,
    const base::Closure& callback,
    const ErrorCallback& error_callback) {
  DBusThreadManager::Get()->GetBluetoothAdapterClient()->
      GetProperties(object_path_)->powered.Set(
          powered,
          base::Bind(&BluetoothAdapterChromeOS::OnPropertyChangeCompleted,
                     weak_ptr_factory_.GetWeakPtr(),
                     callback,
                     error_callback));
}
