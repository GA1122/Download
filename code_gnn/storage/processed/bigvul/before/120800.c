void BluetoothDeviceChromeOS::Disconnect(const base::Closure& callback,
                                         const ErrorCallback& error_callback) {
  VLOG(1) << object_path_.value() << ": Disconnecting";
  DBusThreadManager::Get()->GetBluetoothDeviceClient()->
      Disconnect(
          object_path_,
          base::Bind(&BluetoothDeviceChromeOS::OnDisconnect,
                     weak_ptr_factory_.GetWeakPtr(),
                     callback),
          base::Bind(&BluetoothDeviceChromeOS::OnDisconnectError,
                     weak_ptr_factory_.GetWeakPtr(),
                     error_callback));
}
