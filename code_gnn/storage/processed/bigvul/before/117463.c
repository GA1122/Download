void TestingAutomationProvider::DisconnectFromWifiNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  const chromeos::WifiNetwork* wifi = network_library->wifi_network();
  if (!wifi) {
    reply.SendError("Not connected to any wifi network.");
    return;
  }

  network_library->DisconnectFromNetwork(wifi);
  reply.SendSuccess(NULL);
}
