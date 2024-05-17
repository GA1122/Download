void TestingAutomationProvider::DisconnectFromPrivateNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  const chromeos::VirtualNetwork* virt = network_library->virtual_network();
  if (!virt) {
    reply.SendError("Not connected to any virtual network.");
    return;
  }

  network_library->DisconnectFromNetwork(virt);
  reply.SendSuccess(NULL);
}
