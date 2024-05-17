void TestingAutomationProvider::ConnectToWifiNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string service_path, password;
  bool shared;
  if (!args->GetString("service_path", &service_path) ||
      !args->GetString("password", &password) ||
      !args->GetBoolean("shared", &shared)) {
    reply.SendError("Invalid or missing args.");
    return;
  }

  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  chromeos::WifiNetwork* wifi =
      network_library->FindWifiNetworkByPath(service_path);
  if (!wifi) {
    reply.SendError("No network found with specified service path.");
    return;
  }
  if (!password.empty())
    wifi->SetPassphrase(password);

  if (wifi->encryption() == chromeos::SECURITY_NONE)
    shared = true;

  new ServicePathConnectObserver(this, reply_message, service_path);

  network_library->ConnectToWifiNetwork(wifi, shared);
  network_library->RequestNetworkScan();
}
