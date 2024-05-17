void TestingAutomationProvider::ConnectToPrivateNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string service_path;
  if (!args->GetString("service_path", &service_path)) {
    reply.SendError("Invalid or missing args.");
    return;
  }

  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  chromeos::VirtualNetwork* network =
      network_library->FindVirtualNetworkByPath(service_path);
  if (!network) {
    reply.SendError(StringPrintf("No virtual network found: %s",
                                 service_path.c_str()));
    return;
  }
  if (network->NeedMoreInfoToConnect()) {
    reply.SendError("Virtual network is missing info required to connect.");
    return;
  };

  new VirtualConnectObserver(this, reply_message, network->name());
  network_library->ConnectToVirtualNetwork(network);
}
