void ExtensionPrefs::SetAppNotificationClientId(
    const std::string& extension_id,
    const std::string& oauth_client_id) {
  DCHECK(Extension::IdIsValid(extension_id));
  UpdateExtensionPref(extension_id, kPrefAppNotificationClientId,
                      Value::CreateStringValue(oauth_client_id));
}
