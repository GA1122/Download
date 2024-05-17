void AppLauncherHandler::Observe(int type,
                                 const content::NotificationSource& source,
                                 const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_APP_INSTALLED_TO_NTP) {
    highlight_app_id_ = *content::Details<const std::string>(details).ptr();
    if (has_loaded_apps_)
      SetAppToBeHighlighted();
    return;
  }

  if (ignore_changes_ || !has_loaded_apps_)
    return;

  switch (type) {
    case chrome::NOTIFICATION_EXTENSION_LOADED_DEPRECATED: {
      const Extension* extension =
          content::Details<const Extension>(details).ptr();
      if (!extension->is_app())
        return;

      if (!extensions::ui_util::ShouldDisplayInNewTabPage(
              extension, Profile::FromWebUI(web_ui()))) {
        return;
      }

      scoped_ptr<base::DictionaryValue> app_info(GetAppInfo(extension));
      if (app_info.get()) {
        visible_apps_.insert(extension->id());

        ExtensionPrefs* prefs =
            ExtensionPrefs::Get(extension_service_->profile());
        scoped_ptr<base::FundamentalValue> highlight(
            base::Value::CreateBooleanValue(
                prefs->IsFromBookmark(extension->id()) &&
                attempted_bookmark_app_install_));
        attempted_bookmark_app_install_ = false;
        web_ui()->CallJavascriptFunction(
            "ntp.appAdded", *app_info, *highlight);
      }

      break;
    }
    case chrome::NOTIFICATION_EXTENSION_UNLOADED_DEPRECATED:
    case chrome::NOTIFICATION_EXTENSION_UNINSTALLED: {
      const Extension* extension = NULL;
      bool uninstalled = false;
      if (type == chrome::NOTIFICATION_EXTENSION_UNINSTALLED) {
        extension = content::Details<const Extension>(details).ptr();
        uninstalled = true;
      } else {   
        if (content::Details<UnloadedExtensionInfo>(details)->reason ==
            UnloadedExtensionInfo::REASON_UNINSTALL) {
          return;
        }
        extension = content::Details<extensions::UnloadedExtensionInfo>(
            details)->extension;
        uninstalled = false;
      }
      if (!extension->is_app())
        return;

      if (!extensions::ui_util::ShouldDisplayInNewTabPage(
              extension, Profile::FromWebUI(web_ui()))) {
        return;
      }

      scoped_ptr<base::DictionaryValue> app_info(GetAppInfo(extension));
      if (app_info.get()) {
        if (uninstalled)
          visible_apps_.erase(extension->id());

        scoped_ptr<base::FundamentalValue> uninstall_value(
            base::Value::CreateBooleanValue(uninstalled));
        scoped_ptr<base::FundamentalValue> from_page(
            base::Value::CreateBooleanValue(!extension_id_prompting_.empty()));
        web_ui()->CallJavascriptFunction(
            "ntp.appRemoved", *app_info, *uninstall_value, *from_page);
      }
      break;
    }
    case chrome::NOTIFICATION_EXTENSION_LAUNCHER_REORDERED: {
      const std::string* id =
          content::Details<const std::string>(details).ptr();
      if (id) {
        const Extension* extension =
            extension_service_->GetInstalledExtension(*id);
        if (!extension) {
          return;
        }

        base::DictionaryValue app_info;
        CreateAppInfo(extension,
                      extension_service_,
                      &app_info);
        web_ui()->CallJavascriptFunction("ntp.appMoved", app_info);
      } else {
        HandleGetApps(NULL);
      }
      break;
    }
    case chrome::NOTIFICATION_EXTENSION_INSTALL_ERROR: {
      CrxInstaller* crx_installer = content::Source<CrxInstaller>(source).ptr();
      if (!Profile::FromWebUI(web_ui())->IsSameProfile(
              crx_installer->profile())) {
        return;
      }
    }
    case chrome::NOTIFICATION_EXTENSION_LOAD_ERROR: {
      attempted_bookmark_app_install_ = false;
      break;
    }
    default:
      NOTREACHED();
  }
}
