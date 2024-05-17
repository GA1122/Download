void TestingAutomationProvider::GetPrivateNetworkInfo(
    DictionaryValue* args, IPC::Message* reply_message) {
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();
  const chromeos::VirtualNetworkVector& virtual_networks =
      network_library->virtual_networks();

  if (network_library->virtual_network())
    return_value->SetString("connected",
                            network_library->virtual_network()->service_path());
  for (chromeos::VirtualNetworkVector::const_iterator iter =
       virtual_networks.begin(); iter != virtual_networks.end(); ++iter) {
    const chromeos::VirtualNetwork* virt = *iter;
    DictionaryValue* item = new DictionaryValue;
    item->SetString("name", virt->name());
    item->SetString("provider_type",
                    VPNProviderTypeToString(virt->provider_type()));
    item->SetString("hostname", virt->server_hostname());
    item->SetString("key", virt->psk_passphrase());
    item->SetString("cert_nss", virt->ca_cert_nss());
    item->SetString("cert_id", virt->client_cert_id());
    item->SetString("username", virt->username());
    item->SetString("password", virt->user_passphrase());
    return_value->Set(virt->service_path(), item);
  }

  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
