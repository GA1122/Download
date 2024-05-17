void TestingAutomationProvider::SetExtensionStateById(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  const Extension* extension;
  std::string error;
  if (!GetExtensionFromJSONArgs(args, "id", profile(), &extension, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  bool enable;
  if (!args->GetBoolean("enable", &enable)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Missing or invalid key: enable");
    return;
  }

  bool allow_in_incognito;
  if (!args->GetBoolean("allow_in_incognito", &allow_in_incognito)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Missing or invalid key: allow_in_incognito");
    return;
  }

  if (allow_in_incognito && !enable) {
    AutomationJSONReply(this, reply_message)
        .SendError("Invalid state: Disabled extension "
                    "cannot be allowed in incognito mode.");
    return;
  }

  ExtensionService* service = profile()->GetExtensionService();
  ExtensionProcessManager* manager = profile()->GetExtensionProcessManager();
  if (!service) {
    AutomationJSONReply(this, reply_message)
        .SendError("No extensions service or process manager.");
    return;
  }

  if (enable) {
    if (!service->IsExtensionEnabled(extension->id())) {
      new ExtensionReadyNotificationObserver(
          manager,
          service,
          this,
          reply_message);
      service->EnableExtension(extension->id());
    } else {
      AutomationJSONReply(this, reply_message).SendSuccess(NULL);
    }
  } else {
    service->DisableExtension(extension->id());
    AutomationJSONReply(this, reply_message).SendSuccess(NULL);
  }

  service->SetIsIncognitoEnabled(extension->id(), allow_in_incognito);
}
