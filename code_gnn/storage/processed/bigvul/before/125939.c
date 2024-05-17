ExtensionUninstallObserver::ExtensionUninstallObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    const std::string& id)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      id_(id) {
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNINSTALLED,
                 content::NotificationService::AllSources());
  registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNINSTALL_NOT_ALLOWED,
                 content::NotificationService::AllSources());
}
