std::vector<DictionaryValue*>* GetAppInfoFromExtensions(
    const ExtensionSet* extensions,
    ExtensionService* ext_service) {
  std::vector<DictionaryValue*>* apps_list =
      new std::vector<DictionaryValue*>();
  for (ExtensionSet::const_iterator ext = extensions->begin();
       ext != extensions->end(); ++ext) {
    if ((*ext)->is_app()) {
      DictionaryValue* app_info = new DictionaryValue();
      AppLauncherHandler::CreateAppInfo(*ext, NULL, ext_service, app_info);
      app_info->SetBoolean("is_component_extension",
          (*ext)->location() == extensions::Extension::COMPONENT);

      int launch_type;
      const char* kLaunchType = "launch_type";
      if (!app_info->GetInteger(kLaunchType, &launch_type)) {
        NOTREACHED() << "Can't get integer from key " << kLaunchType;
        continue;
      }
      if (launch_type == extensions::ExtensionPrefs::LAUNCH_PINNED) {
        app_info->SetString(kLaunchType, "pinned");
      } else if (launch_type == extensions::ExtensionPrefs::LAUNCH_REGULAR) {
        app_info->SetString(kLaunchType, "regular");
      } else if (launch_type == extensions::ExtensionPrefs::LAUNCH_FULLSCREEN) {
        app_info->SetString(kLaunchType, "fullscreen");
      } else if (launch_type == extensions::ExtensionPrefs::LAUNCH_WINDOW) {
        app_info->SetString(kLaunchType, "window");
      } else {
        app_info->SetString(kLaunchType, "unknown");
      }

      apps_list->push_back(app_info);
    }
  }
  return apps_list;
}
