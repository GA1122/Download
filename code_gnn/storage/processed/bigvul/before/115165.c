void BluetoothOptionsHandler::DiscoveryEnded(const std::string& adapter_id) {
  VLOG(2) << "Discovery ended on " << adapter_id;
  web_ui_->CallJavascriptFunction(
      "options.SystemOptions.notifyBluetoothSearchComplete");

  chromeos::BluetoothManager* bluetooth_manager =
      chromeos::BluetoothManager::GetInstance();
  DCHECK(bluetooth_manager);

  chromeos::BluetoothAdapter* default_adapter =
      bluetooth_manager->DefaultAdapter();

  ValidateDefaultAdapter(default_adapter);

  if (default_adapter == NULL) {
    VLOG(1) << "DiscoveryEnded: no default adapter";
    return;
  }

  default_adapter->StopDiscovery();
}
