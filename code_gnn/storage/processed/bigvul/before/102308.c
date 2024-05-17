void ExtensionPrefs::RemoveExtensionControlledPref(
    const std::string& extension_id,
    const std::string& pref_key,
    ExtensionPrefsScope scope) {
  DCHECK(pref_service()->FindPreference(pref_key.c_str()))
      << "Extension controlled preference key " << pref_key
      << " not registered.";

  if (scope == kExtensionPrefsScopeRegular) {
    ScopedExtensionControlledPrefUpdate update(prefs_, extension_id,
                                               kPrefPreferences);
    update->RemoveWithoutPathExpansion(pref_key, NULL);
  } else if (scope == kExtensionPrefsScopeIncognitoPersistent) {
    ScopedExtensionControlledPrefUpdate update(prefs_, extension_id,
                                               kPrefIncognitoPreferences);
    update->RemoveWithoutPathExpansion(pref_key, NULL);
  }

  extension_pref_value_map_->RemoveExtensionPref(
      extension_id, pref_key, scope);
}
