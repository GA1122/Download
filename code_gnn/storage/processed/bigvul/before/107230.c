void AutomationProvider::EnableExtension(int extension_handle,
                                         IPC::Message* reply_message) {
  const Extension* extension = GetDisabledExtension(extension_handle);
  ExtensionService* service = profile_->GetExtensionService();
  ExtensionProcessManager* manager = profile_->GetExtensionProcessManager();
  if (extension && service && manager) {
    new ExtensionReadyNotificationObserver(
        manager,
        this,
        AutomationMsg_EnableExtension::ID,
        reply_message);
    service->EnableExtension(extension->id());
  } else {
    AutomationMsg_EnableExtension::WriteReplyParams(reply_message, false);
    Send(reply_message);
  }
}
