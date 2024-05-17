void ExtensionPrefs::OnContentSettingChanged(
    const std::string& extension_id,
    bool incognito) {
  if (incognito) {
    UpdateExtensionPref(
        extension_id, kPrefIncognitoContentSettings,
        content_settings_store_->GetSettingsForExtension(
            extension_id, kExtensionPrefsScopeIncognitoPersistent));
  } else {
    UpdateExtensionPref(
        extension_id, kPrefContentSettings,
        content_settings_store_->GetSettingsForExtension(
            extension_id, kExtensionPrefsScopeRegular));
  }
}
