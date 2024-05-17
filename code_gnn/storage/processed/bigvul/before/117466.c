void TestingAutomationProvider::ForgetWifiNetwork(
    DictionaryValue* args, IPC::Message* reply_message) {
  std::string service_path;
  if (!args->GetString("service_path", &service_path)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing args.");
    return;
  }

  CrosLibrary::Get()->GetNetworkLibrary()->ForgetNetwork(service_path);
  AutomationJSONReply(this, reply_message).SendSuccess(NULL);
}
