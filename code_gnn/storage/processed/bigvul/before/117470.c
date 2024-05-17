void TestingAutomationProvider::GetNetworkInfo(DictionaryValue* args,
                                               IPC::Message* reply_message) {
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();

  return_value->SetBoolean("offline_mode",
                           net::NetworkChangeNotifier::IsOffline());

  return_value->SetString("ip_address", network_library->IPAddress());

  if (network_library->ethernet_network())
    return_value->SetString(
        "connected_ethernet",
        network_library->ethernet_network()->service_path());
  if (network_library->wifi_network())
    return_value->SetString("connected_wifi",
                            network_library->wifi_network()->service_path());
  if (network_library->cellular_network())
    return_value->SetString(
        "connected_cellular",
        network_library->cellular_network()->service_path());

  bool ethernet_available = network_library->ethernet_available();
  bool ethernet_enabled = network_library->ethernet_enabled();
  return_value->SetBoolean("ethernet_available", ethernet_available);
  return_value->SetBoolean("ethernet_enabled", ethernet_enabled);
  if (ethernet_available && ethernet_enabled) {
    const chromeos::EthernetNetwork* ethernet_network =
        network_library->ethernet_network();
    if (ethernet_network) {
      DictionaryValue* items = new DictionaryValue;
      DictionaryValue* item = GetNetworkInfoDict(ethernet_network);
      items->Set(ethernet_network->service_path(), item);
      return_value->Set("ethernet_networks", items);
    }
  }

  bool wifi_available = network_library->wifi_available();
  bool wifi_enabled = network_library->wifi_enabled();
  return_value->SetBoolean("wifi_available", wifi_available);
  return_value->SetBoolean("wifi_enabled", wifi_enabled);
  if (wifi_available && wifi_enabled) {
    const chromeos::WifiNetworkVector& wifi_networks =
        network_library->wifi_networks();
    DictionaryValue* items = new DictionaryValue;
    for (chromeos::WifiNetworkVector::const_iterator iter =
         wifi_networks.begin(); iter != wifi_networks.end(); ++iter) {
      const chromeos::WifiNetwork* wifi = *iter;
      DictionaryValue* item = GetNetworkInfoDict(wifi);
      item->SetInteger("strength", wifi->strength());
      item->SetBoolean("encrypted", wifi->encrypted());
      item->SetString("encryption", wifi->GetEncryptionString());
      items->Set(wifi->service_path(), item);
    }
    return_value->Set("wifi_networks", items);
  }

  bool cellular_available = network_library->cellular_available();
  bool cellular_enabled = network_library->cellular_enabled();
  return_value->SetBoolean("cellular_available", cellular_available);
  return_value->SetBoolean("cellular_enabled", cellular_enabled);
  if (cellular_available && cellular_enabled) {
    const chromeos::CellularNetworkVector& cellular_networks =
        network_library->cellular_networks();
    DictionaryValue* items = new DictionaryValue;
    for (size_t i = 0; i < cellular_networks.size(); ++i) {
      DictionaryValue* item = GetNetworkInfoDict(cellular_networks[i]);
      item->SetInteger("strength", cellular_networks[i]->strength());
      item->SetString("operator_name", cellular_networks[i]->operator_name());
      item->SetString("operator_code", cellular_networks[i]->operator_code());
      item->SetString("payment_url", cellular_networks[i]->payment_url());
      item->SetString("usage_url", cellular_networks[i]->usage_url());
      item->SetString("network_technology",
                      cellular_networks[i]->GetNetworkTechnologyString());
      item->SetString("activation_state",
                      cellular_networks[i]->GetActivationStateString());
      item->SetString("roaming_state",
                      cellular_networks[i]->GetRoamingStateString());
      items->Set(cellular_networks[i]->service_path(), item);
    }
    return_value->Set("cellular_networks", items);
  }

  const chromeos::WifiNetworkVector& remembered_wifi =
      network_library->remembered_wifi_networks();
  ListValue* items = new ListValue;
  for (chromeos::WifiNetworkVector::const_iterator iter =
       remembered_wifi.begin(); iter != remembered_wifi.end();
       ++iter) {
      const chromeos::WifiNetwork* wifi = *iter;
      items->Append(base::Value::CreateStringValue(wifi->service_path()));
  }
  return_value->Set("remembered_wifi", items);

  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
