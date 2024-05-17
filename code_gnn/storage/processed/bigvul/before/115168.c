void BluetoothOptionsHandler::FindDevicesCallback(
    const ListValue* args) {
  if (!chromeos::system::runtime_environment::IsRunningOnChromeOS()) {
    GenerateFakeDeviceList();
    return;
  }

  chromeos::BluetoothManager* bluetooth_manager =
      chromeos::BluetoothManager::GetInstance();
  DCHECK(bluetooth_manager);

  chromeos::BluetoothAdapter* default_adapter =
      bluetooth_manager->DefaultAdapter();

  ValidateDefaultAdapter(default_adapter);

  if (default_adapter == NULL) {
    VLOG(1) << "FindDevicesCallback: no default adapter";
    return;
  }

  default_adapter->StartDiscovery();
}
