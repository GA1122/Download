void TestingAutomationProvider::ConnectToHiddenWifiNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  std::string ssid, security, password;
  bool shared;
  if (!args->GetString("ssid", &ssid) ||
      !args->GetString("security", &security) ||
      !args->GetString("password", &password) ||
      !args->GetBoolean("shared", &shared)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing args.");
    return;
  }

  std::map<std::string, chromeos::ConnectionSecurity> connection_security_map;
  connection_security_map["SECURITY_NONE"] = chromeos::SECURITY_NONE;
  connection_security_map["SECURITY_WEP"] = chromeos::SECURITY_WEP;
  connection_security_map["SECURITY_WPA"] = chromeos::SECURITY_WPA;
  connection_security_map["SECURITY_RSN"] = chromeos::SECURITY_RSN;
  connection_security_map["SECURITY_8021X"] = chromeos::SECURITY_8021X;

  if (connection_security_map.find(security) == connection_security_map.end()) {
    AutomationJSONReply(this, reply_message).SendError(
        "Unknown security type.");
    return;
  }
  chromeos::ConnectionSecurity connection_security =
      connection_security_map[security];

  NetworkLibrary* network_library = CrosLibrary::Get()->GetNetworkLibrary();

  new SSIDConnectObserver(this, reply_message, ssid);

  bool save_credentials = false;

  if (connection_security == chromeos::SECURITY_8021X) {
    chromeos::NetworkLibrary::EAPConfigData config_data;
    std::string eap_method, eap_auth, eap_identity;
    if (!args->GetString("eap_method", &eap_method) ||
        !args->GetString("eap_auth", &eap_auth) ||
        !args->GetString("eap_identity", &eap_identity) ||
        !args->GetBoolean("save_credentials", &save_credentials)) {
      AutomationJSONReply(this, reply_message).SendError(
          "Invalid or missing EAP args.");
      return;
    }

    std::map<std::string, chromeos::EAPMethod> eap_method_map;
    eap_method_map["EAP_METHOD_NONE"] = chromeos::EAP_METHOD_UNKNOWN;
    eap_method_map["EAP_METHOD_PEAP"] = chromeos::EAP_METHOD_PEAP;
    eap_method_map["EAP_METHOD_TLS"] = chromeos::EAP_METHOD_TLS;
    eap_method_map["EAP_METHOD_TTLS"] = chromeos::EAP_METHOD_TTLS;
    eap_method_map["EAP_METHOD_LEAP"] = chromeos::EAP_METHOD_LEAP;
    if (eap_method_map.find(eap_method) == eap_method_map.end()) {
      AutomationJSONReply(this, reply_message).SendError(
          "Unknown EAP Method type.");
      return;
    }
    config_data.method = eap_method_map[eap_method];

    std::map<std::string, chromeos::EAPPhase2Auth> eap_auth_map;
    eap_auth_map["EAP_PHASE_2_AUTH_AUTO"] = chromeos::EAP_PHASE_2_AUTH_AUTO;
    eap_auth_map["EAP_PHASE_2_AUTH_MD5"] = chromeos::EAP_PHASE_2_AUTH_MD5;
    eap_auth_map["EAP_PHASE_2_AUTH_MSCHAP"] =
        chromeos::EAP_PHASE_2_AUTH_MSCHAP;
    eap_auth_map["EAP_PHASE_2_AUTH_MSCHAPV2"] =
        chromeos::EAP_PHASE_2_AUTH_MSCHAPV2;
    eap_auth_map["EAP_PHASE_2_AUTH_PAP"] = chromeos::EAP_PHASE_2_AUTH_PAP;
    eap_auth_map["EAP_PHASE_2_AUTH_CHAP"] = chromeos::EAP_PHASE_2_AUTH_CHAP;
    if (eap_auth_map.find(eap_auth) == eap_auth_map.end()) {
      AutomationJSONReply(this, reply_message).SendError(
          "Unknown EAP Phase2 Auth type.");
      return;
    }
    config_data.auth = eap_auth_map[eap_auth];

    config_data.identity = eap_identity;

    config_data.server_ca_cert_nss_nickname = "";
    config_data.use_system_cas = false;
    config_data.client_cert_pkcs11_id = "";

    network_library->ConnectToUnconfiguredWifiNetwork(
        ssid, chromeos::SECURITY_8021X, password, &config_data,
        save_credentials, shared);
  } else {
    network_library->ConnectToUnconfiguredWifiNetwork(
        ssid, connection_security, password, NULL,
        save_credentials, shared);
  }
}
