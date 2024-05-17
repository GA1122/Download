void ExtensionReadyNotificationObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }

  switch (type) {
    case content::NOTIFICATION_LOAD_STOP:
      if (!extension_ || !DidExtensionViewsStopLoading(manager_))
        return;
      break;
    case chrome::NOTIFICATION_EXTENSION_LOADED: {
      const extensions::Extension* loaded_extension =
          content::Details<const extensions::Extension>(details).ptr();
      extensions::Extension::Location location = loaded_extension->location();
      if (location != extensions::Extension::INTERNAL &&
          location != extensions::Extension::LOAD)
        return;
      extension_ = loaded_extension;
      if (!DidExtensionViewsStopLoading(manager_))
        return;
      if (!service_->IsBackgroundPageReady(extension_))
        return;
      break;
    }
    case chrome::NOTIFICATION_EXTENSION_INSTALL_ERROR:
    case chrome::NOTIFICATION_EXTENSION_LOAD_ERROR:
    case chrome::NOTIFICATION_EXTENSION_UPDATE_DISABLED:
      break;
    default:
      NOTREACHED();
      break;
  }

  AutomationJSONReply reply(automation_, reply_message_.release());
  if (extension_) {
    DictionaryValue dict;
    dict.SetString("id", extension_->id());
    reply.SendSuccess(&dict);
  } else {
    reply.SendError("Extension could not be installed");
  }
  delete this;
}
