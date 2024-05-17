void BluetoothAdapterChromeOS::SetName(const std::string& name,
                                       const base::Closure& callback,
                                       const ErrorCallback& error_callback) {
  DBusThreadManager::Get()->GetBluetoothAdapterClient()->
      GetProperties(object_path_)->alias.Set(
          name,
          base::Bind(&BluetoothAdapterChromeOS::OnPropertyChangeCompleted,
                     weak_ptr_factory_.GetWeakPtr(),
                     callback,
                     error_callback));
}
