void BluetoothAdapterChromeOS::StopDiscovering(
    const base::Closure& callback,
    const ErrorCallback& error_callback) {
  DBusThreadManager::Get()->GetBluetoothAdapterClient()->
      StopDiscovery(
          object_path_,
          base::Bind(&BluetoothAdapterChromeOS::OnStopDiscovery,
                     weak_ptr_factory_.GetWeakPtr(),
                     callback),
          base::Bind(&BluetoothAdapterChromeOS::OnStopDiscoveryError,
                     weak_ptr_factory_.GetWeakPtr(),
                     error_callback));
}
