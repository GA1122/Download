void ExtensionPrefs::OnExtensionInstalled(
    const Extension* extension,
    Extension::State initial_state,
    bool from_webstore,
    const StringOrdinal& page_ordinal) {
  const std::string& id = extension->id();
  CHECK(Extension::IdIsValid(id));
  ScopedExtensionPrefUpdate update(prefs_, id);
  DictionaryValue* extension_dict = update.Get();
  const base::Time install_time = GetCurrentTime();
  extension_dict->Set(kPrefState, Value::CreateIntegerValue(initial_state));
  extension_dict->Set(kPrefLocation,
                      Value::CreateIntegerValue(extension->location()));
  extension_dict->Set(kPrefFromWebStore,
                      Value::CreateBooleanValue(from_webstore));
  extension_dict->Set(kPrefFromBookmark,
                      Value::CreateBooleanValue(extension->from_bookmark()));
  extension_dict->Set(kPrefInstallTime,
                      Value::CreateStringValue(
                          base::Int64ToString(install_time.ToInternalValue())));
  extension_dict->Set(kPrefPreferences, new DictionaryValue());
  extension_dict->Set(kPrefIncognitoPreferences, new DictionaryValue());
  extension_dict->Set(kPrefContentSettings, new ListValue());
  extension_dict->Set(kPrefIncognitoContentSettings, new ListValue());

  FilePath::StringType path = MakePathRelative(install_directory_,
      extension->path());
  extension_dict->Set(kPrefPath, Value::CreateStringValue(path));
  if (extension->location() != Extension::LOAD) {
    extension_dict->Set(kPrefManifest,
                        extension->manifest()->value()->DeepCopy());
  }

  if (extension->is_app()) {
    StringOrdinal new_page_ordinal =
        page_ordinal.IsValid() ? page_ordinal : GetNaturalAppPageOrdinal();
    SetPageOrdinal(id, new_page_ordinal);
    SetAppLaunchOrdinal(id, CreateNextAppLaunchOrdinal(new_page_ordinal));
  }

  extension_pref_value_map_->RegisterExtension(
      id, install_time, initial_state == Extension::ENABLED);
  content_settings_store_->RegisterExtension(
      id, install_time, initial_state == Extension::ENABLED);
}
