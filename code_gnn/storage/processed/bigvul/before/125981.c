void ExtensionsUpdatedObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (!automation_) {
    delete this;
    return;
  }

  switch (type) {
    case chrome::NOTIFICATION_EXTENSION_UPDATE_FOUND:
      in_progress_updates_.insert(
          *(content::Details<const std::string>(details).ptr()));
      break;

    case chrome::NOTIFICATION_EXTENSION_UPDATING_FINISHED:
      updater_finished_ = true;
      break;

    case chrome::NOTIFICATION_EXTENSION_LOADED:
    case chrome::NOTIFICATION_EXTENSION_INSTALL_NOT_ALLOWED:
    case chrome::NOTIFICATION_EXTENSION_UPDATE_DISABLED: {
      const extensions::Extension* extension =
          content::Details<extensions::Extension>(details).ptr();
      in_progress_updates_.erase(extension->id());
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_INSTALL_ERROR: {
      extensions::CrxInstaller* installer =
          content::Source<extensions::CrxInstaller>(source).ptr();
      in_progress_updates_.erase(installer->expected_id());
      break;
    }

    case content::NOTIFICATION_LOAD_STOP:
      break;

    default:
      NOTREACHED();
      break;
  }

  if (updater_finished_ && in_progress_updates_.empty() &&
      DidExtensionViewsStopLoading(manager_)) {
    AutomationJSONReply reply(automation_, reply_message_.release());
    reply.SendSuccess(NULL);
    delete this;
  }
}
