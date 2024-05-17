void ExtensionPrefs::InitPrefStore(bool extensions_disabled) {
  if (extensions_disabled) {
    extension_pref_value_map_->NotifyInitializationCompleted();
    return;
  }

  ExtensionIdSet extension_ids;
  GetExtensions(&extension_ids);
  for (ExtensionIdSet::iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    ScopedExtensionPrefUpdate update(prefs_, *ext_id);
    update.Get();
  }

  FixMissingPrefs(extension_ids);
  InitializePageOrdinalMap(extension_ids);
  MigratePermissions(extension_ids);
  MigrateAppIndex(extension_ids);

  for (ExtensionIdSet::iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    extension_pref_value_map_->RegisterExtension(
        *ext_id,
        GetInstallTime(*ext_id),
        !IsExtensionDisabled(*ext_id));
    content_settings_store_->RegisterExtension(
        *ext_id,
        GetInstallTime(*ext_id),
        !IsExtensionDisabled(*ext_id));

    const DictionaryValue* prefs = GetExtensionControlledPrefs(*ext_id, false);
    for (DictionaryValue::Iterator i(*prefs); i.HasNext(); i.Advance()) {
      extension_pref_value_map_->SetExtensionPref(
          *ext_id, i.key(), kExtensionPrefsScopeRegular, i.value().DeepCopy());
    }

    prefs = GetExtensionControlledPrefs(*ext_id, true);
    for (DictionaryValue::Iterator i(*prefs); i.HasNext(); i.Advance()) {
      extension_pref_value_map_->SetExtensionPref(
          *ext_id, i.key(), kExtensionPrefsScopeIncognitoPersistent,
          i.value().DeepCopy());
    }

    const DictionaryValue* extension_prefs = GetExtensionPref(*ext_id);
    DCHECK(extension_prefs);
    ListValue* content_settings = NULL;
    if (extension_prefs->GetList(kPrefContentSettings,
                                 &content_settings)) {
      content_settings_store_->SetExtensionContentSettingsFromList(
          *ext_id, content_settings,
          kExtensionPrefsScopeRegular);
    }
    if (extension_prefs->GetList(kPrefIncognitoContentSettings,
                                 &content_settings)) {
      content_settings_store_->SetExtensionContentSettingsFromList(
          *ext_id, content_settings,
          kExtensionPrefsScopeIncognitoPersistent);
    }
  }

  extension_pref_value_map_->NotifyInitializationCompleted();
}
