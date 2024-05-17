void TestingAutomationProvider::NetworkScan(DictionaryValue* args,
                                            IPC::Message* reply_message) {
  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  network_library->RequestNetworkScan();

  new NetworkScanObserver(this, reply_message);
}
