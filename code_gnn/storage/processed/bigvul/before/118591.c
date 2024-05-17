void AppLauncherHandler::HandleGetApps(const base::ListValue* args) {
  base::DictionaryValue dictionary;

  Profile* profile = Profile::FromWebUI(web_ui());

  if (!has_loaded_apps_) {
    ExtensionRegistry* registry = ExtensionRegistry::Get(profile);
    const ExtensionSet& enabled_set = registry->enabled_extensions();
    for (extensions::ExtensionSet::const_iterator it = enabled_set.begin();
         it != enabled_set.end(); ++it) {
      visible_apps_.insert((*it)->id());
    }

    const ExtensionSet& disabled_set = registry->disabled_extensions();
    for (ExtensionSet::const_iterator it = disabled_set.begin();
         it != disabled_set.end(); ++it) {
      visible_apps_.insert((*it)->id());
    }

    const ExtensionSet& terminated_set = registry->terminated_extensions();
    for (ExtensionSet::const_iterator it = terminated_set.begin();
         it != terminated_set.end(); ++it) {
      visible_apps_.insert((*it)->id());
    }
  }

  SetAppToBeHighlighted();
  FillAppDictionary(&dictionary);
  web_ui()->CallJavascriptFunction("ntp.getAppsCallback", dictionary);

  if (!has_loaded_apps_) {
    base::Closure callback = base::Bind(
        &AppLauncherHandler::OnExtensionPreferenceChanged,
        base::Unretained(this));
    extension_pref_change_registrar_.Init(
        ExtensionPrefs::Get(profile)->pref_service());
    extension_pref_change_registrar_.Add(
        extensions::pref_names::kExtensions, callback);
    extension_pref_change_registrar_.Add(prefs::kNtpAppPageNames, callback);

    registrar_.Add(this,
                   chrome::NOTIFICATION_EXTENSION_LOADED_DEPRECATED,
                   content::Source<Profile>(profile));
    registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNLOADED_DEPRECATED,
        content::Source<Profile>(profile));
    registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_UNINSTALLED,
        content::Source<Profile>(profile));
    registrar_.Add(this,
                   chrome::NOTIFICATION_EXTENSION_LAUNCHER_REORDERED,
                   content::Source<AppSorting>(
                       ExtensionPrefs::Get(profile)->app_sorting()));
    registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_INSTALL_ERROR,
        content::Source<CrxInstaller>(NULL));
    registrar_.Add(this, chrome::NOTIFICATION_EXTENSION_LOAD_ERROR,
        content::Source<Profile>(profile));
  }

  has_loaded_apps_ = true;
}
