void TestingAutomationProvider::ConnectToCellularNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  std::string service_path;
  if (!args->GetString("service_path", &service_path)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing args.");
    return;
  }

  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  chromeos::CellularNetwork* cellular =
      network_library->FindCellularNetworkByPath(service_path);
  if (!cellular) {
    AutomationJSONReply(this, reply_message).SendError(
        "No network found with specified service path.");
    return;
  }

  new ServicePathConnectObserver(this, reply_message, service_path);

  network_library->ConnectToCellularNetwork(cellular);
  network_library->RequestNetworkScan();
}
