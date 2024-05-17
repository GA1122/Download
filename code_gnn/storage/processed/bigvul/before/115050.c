void TestingAutomationProvider::UninstallExtensionById(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  const Extension* extension;
  std::string error;
  if (!GetExtensionFromJSONArgs(args, "id", profile(), &extension, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  ExtensionService* service = profile()->GetExtensionService();
  if (!service) {
    AutomationJSONReply(this, reply_message).SendError(
        "No extensions service.");
    return;
  }

  new ExtensionUninstallObserver(this, reply_message, extension->id());
  service->UninstallExtension(extension->id(), false, NULL);
}
