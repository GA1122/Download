void BluetoothDeviceChromeOS::SetTrusted() {
  DBusThreadManager::Get()->GetBluetoothDeviceClient()->
      GetProperties(object_path_)->trusted.Set(
          true,
          base::Bind(&BluetoothDeviceChromeOS::OnSetTrusted,
                     weak_ptr_factory_.GetWeakPtr()));
}
