void ExtensionPrefs::SetExtensionControlledPref(
    const std::string& extension_id,
    const std::string& pref_key,
    ExtensionPrefsScope scope,
    Value* value) {
#ifndef NDEBUG
  const PrefService::Preference* pref =
      pref_service()->FindPreference(pref_key.c_str());
  DCHECK(pref) << "Extension controlled preference key " << pref_key
               << " not registered.";
  DCHECK_EQ(pref->GetType(), value->GetType())
      << "Extension controlled preference " << pref_key << " has wrong type.";
#endif

  if (scope == kExtensionPrefsScopeRegular) {
    ScopedExtensionControlledPrefUpdate update(prefs_, extension_id,
                                               kPrefPreferences);
    update->SetWithoutPathExpansion(pref_key, value->DeepCopy());
  } else if (scope == kExtensionPrefsScopeIncognitoPersistent) {
    ScopedExtensionControlledPrefUpdate update(prefs_, extension_id,
                                               kPrefIncognitoPreferences);
    update->SetWithoutPathExpansion(pref_key, value->DeepCopy());
  }

  extension_pref_value_map_->SetExtensionPref(
      extension_id, pref_key, scope, value);
}
