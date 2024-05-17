void TestingAutomationProvider::DisconnectFromCellularNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  const chromeos::CellularNetwork* cellular =
        network_library->cellular_network();
  if (!cellular) {
    AutomationJSONReply(this, reply_message).SendError(
        "Not connected to any cellular network.");
    return;
  }

  new NetworkDisconnectObserver(this, reply_message, cellular->service_path());

  network_library->DisconnectFromNetwork(cellular);
}
