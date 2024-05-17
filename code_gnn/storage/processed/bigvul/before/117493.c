void TestingAutomationProvider::ToggleNetworkDevice(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string device;
  bool enable;
  if (!args->GetString("device", &device) ||
      !args->GetBoolean("enable", &enable)) {
    reply.SendError("Invalid or missing args.");
    return;
  }

  new ToggleNetworkDeviceObserver(this, reply_message, device, enable);

  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  if (device == "ethernet") {
    network_library->EnableEthernetNetworkDevice(enable);
  } else if (device == "wifi") {
    network_library->EnableWifiNetworkDevice(enable);
  } else if (device == "cellular") {
    network_library->EnableCellularNetworkDevice(enable);
  } else {
    reply.SendError(
        "Unknown device. Valid devices are ethernet, wifi, cellular.");
    return;
  }
}
