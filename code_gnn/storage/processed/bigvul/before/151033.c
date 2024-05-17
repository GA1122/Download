void DevToolsUIBindings::SetDevicesDiscoveryConfig(
    bool discover_usb_devices,
    bool port_forwarding_enabled,
    const std::string& port_forwarding_config) {
  base::DictionaryValue* config_dict = nullptr;
  std::unique_ptr<base::Value> parsed_config =
      base::JSONReader::Read(port_forwarding_config);
  if (!parsed_config || !parsed_config->GetAsDictionary(&config_dict))
    return;

  profile_->GetPrefs()->SetBoolean(
      prefs::kDevToolsDiscoverUsbDevicesEnabled, discover_usb_devices);
  profile_->GetPrefs()->SetBoolean(
      prefs::kDevToolsPortForwardingEnabled, port_forwarding_enabled);
  profile_->GetPrefs()->Set(
      prefs::kDevToolsPortForwardingConfig, *config_dict);
}
