bool BluetoothDeviceChromeOS::IsConnectable() const {
  BluetoothInputClient::Properties* input_properties =
      DBusThreadManager::Get()->GetBluetoothInputClient()->
          GetProperties(object_path_);
  if (!input_properties)
    return true;

  return input_properties->reconnect_mode.value() != "device";
}
