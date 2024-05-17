void TestingAutomationProvider::AddPrivateNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  std::string hostname, service_name, provider_type, key, cert_id, cert_nss,
      username, password;
  if (!args->GetString("hostname", &hostname) ||
      !args->GetString("service_name", &service_name) ||
      !args->GetString("provider_type", &provider_type) ||
      !args->GetString("username", &username) ||
      !args->GetString("password", &password)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Invalid or missing args.");
    return;
  }

  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();

  if (provider_type == VPNProviderTypeToString(
      chromeos::PROVIDER_TYPE_L2TP_IPSEC_PSK)) {
    if (!args->GetString("key", &key)) {
      AutomationJSONReply(this, reply_message)
          .SendError("Missing key arg.");
      return;
    }
    new VirtualConnectObserver(this, reply_message, service_name);
    chromeos::NetworkLibrary::VPNConfigData config_data;
    config_data.psk = key;
    config_data.username = username;
    config_data.user_passphrase = password;
    network_library->ConnectToUnconfiguredVirtualNetwork(
        service_name,
        hostname,
        chromeos::PROVIDER_TYPE_L2TP_IPSEC_PSK,
        config_data);
  } else if (provider_type == VPNProviderTypeToString(
      chromeos::PROVIDER_TYPE_L2TP_IPSEC_USER_CERT)) {
    if (!args->GetString("cert_id", &cert_id) ||
        !args->GetString("cert_nss", &cert_nss)) {
      AutomationJSONReply(this, reply_message)
          .SendError("Missing a certificate arg.");
      return;
    }
    new VirtualConnectObserver(this, reply_message, service_name);
    chromeos::NetworkLibrary::VPNConfigData config_data;
    config_data.server_ca_cert_nss_nickname = cert_nss;
    config_data.client_cert_pkcs11_id = cert_id;
    config_data.username = username;
    config_data.user_passphrase = password;
    network_library->ConnectToUnconfiguredVirtualNetwork(
        service_name,
        hostname,
        chromeos::PROVIDER_TYPE_L2TP_IPSEC_USER_CERT,
        config_data);
  } else if (provider_type == VPNProviderTypeToString(
      chromeos::PROVIDER_TYPE_OPEN_VPN)) {
    std::string otp;
    args->GetString("otp", &otp);
    chromeos::NetworkLibrary::VPNConfigData config_data;
    config_data.server_ca_cert_nss_nickname = cert_nss;
    config_data.client_cert_pkcs11_id = cert_id;
    config_data.username = username;
    config_data.user_passphrase = password;
    config_data.otp = otp;
    network_library->ConnectToUnconfiguredVirtualNetwork(
        service_name,
        hostname,
        chromeos::PROVIDER_TYPE_OPEN_VPN,
        config_data);
  } else {
    AutomationJSONReply(this, reply_message)
        .SendError("Unsupported provider type.");
    return;
  }
}
