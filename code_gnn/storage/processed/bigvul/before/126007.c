void ExtensionPopupObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }

  extensions::ExtensionHost* host =
      content::Details<extensions::ExtensionHost>(details).ptr();
  if (host->extension_id() == extension_id_ &&
      host->extension_host_type() == chrome::VIEW_TYPE_EXTENSION_POPUP) {
    AutomationJSONReply(automation_, reply_message_.release())
        .SendSuccess(NULL);
    delete this;
  }
}
