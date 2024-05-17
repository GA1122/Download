void ExtensionPrefs::ClearIncognitoSessionOnlyContentSettings() {
  ExtensionIdSet extension_ids;
  GetExtensions(&extension_ids);
  for (ExtensionIdSet::iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    content_settings_store_->ClearContentSettingsForExtension(
        *ext_id,
        kExtensionPrefsScopeIncognitoSessionOnly);
  }
}
