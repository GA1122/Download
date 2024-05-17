 void BluetoothDeviceChromeOS::ConnectInternal(
     bool after_pairing,
     const base::Closure& callback,
    const ConnectErrorCallback& error_callback) {
  VLOG(1) << object_path_.value() << ": Connecting";
  DBusThreadManager::Get()->GetBluetoothDeviceClient()->
      Connect(
          object_path_,
          base::Bind(&BluetoothDeviceChromeOS::OnConnect,
                     weak_ptr_factory_.GetWeakPtr(),
                     after_pairing,
                     callback),
          base::Bind(&BluetoothDeviceChromeOS::OnConnectError,
                     weak_ptr_factory_.GetWeakPtr(),
                     after_pairing,
                     error_callback));
}
