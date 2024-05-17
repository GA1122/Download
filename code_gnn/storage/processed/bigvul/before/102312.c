  ScopedExtensionControlledPrefUpdate(
      PrefService* service,
      const std::string& extension_id,
      const std::string& incognito_or_regular_path) :
    DictionaryPrefUpdate(service, ExtensionPrefs::kExtensionsPref),
    prefs_(service),
    extension_id_(extension_id),
    incognito_or_regular_path_(incognito_or_regular_path) {}
