void ExtensionUninstallObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }

  switch (type) {
    case chrome::NOTIFICATION_EXTENSION_UNINSTALLED: {
      if (id_ == content::Details<extensions::Extension>(details).ptr()->id()) {
        scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
        return_value->SetBoolean("success", true);
        AutomationJSONReply(automation_, reply_message_.release())
            .SendSuccess(return_value.get());
        delete this;
        return;
      }
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_UNINSTALL_NOT_ALLOWED: {
      const extensions::Extension* extension =
          content::Details<extensions::Extension>(details).ptr();
      if (id_ == extension->id()) {
        scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
        return_value->SetBoolean("success", false);
        AutomationJSONReply(automation_, reply_message_.release())
            .SendSuccess(return_value.get());
        delete this;
        return;
      }
      break;
    }

    default:
      NOTREACHED();
  }
}
