void BluetoothDeviceChromeOS::ConnectToProfile(
    device::BluetoothProfile* profile,
    const base::Closure& callback,
    const ErrorCallback& error_callback) {
  BluetoothProfileChromeOS* profile_chromeos =
      static_cast<BluetoothProfileChromeOS*>(profile);
  VLOG(1) << object_path_.value() << ": Connecting profile: "
          << profile_chromeos->uuid();
  DBusThreadManager::Get()->GetBluetoothDeviceClient()->
      ConnectProfile(
          object_path_,
          profile_chromeos->uuid(),
          base::Bind(
              &BluetoothDeviceChromeOS::OnConnectProfile,
              weak_ptr_factory_.GetWeakPtr(),
              profile,
              callback),
          base::Bind(
              &BluetoothDeviceChromeOS::OnConnectProfileError,
              weak_ptr_factory_.GetWeakPtr(),
              profile,
              error_callback));
}
