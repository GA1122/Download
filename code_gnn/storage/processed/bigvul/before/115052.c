void TestingAutomationProvider::UpdateExtensionsNow(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  ExtensionService* service = profile()->GetExtensionService();
  if (!service) {
    AutomationJSONReply(this, reply_message).SendError(
        "No extensions service.");
    return;
  }

  ExtensionUpdater* updater = service->updater();
  if (!updater) {
    AutomationJSONReply(this, reply_message).SendError(
        "No updater for extensions service.");
    return;
  }

  ExtensionProcessManager* manager = profile()->GetExtensionProcessManager();
  if (!manager) {
    AutomationJSONReply(this, reply_message).SendError(
        "No extension process manager.");
    return;
  }

  new ExtensionsUpdatedObserver(manager, this, reply_message);
  updater->CheckNow();
}
