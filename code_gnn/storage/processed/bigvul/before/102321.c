void ExtensionPrefs::SetAppNotificationDisabled(
   const std::string& extension_id, bool value) {
  DCHECK(Extension::IdIsValid(extension_id));
  UpdateExtensionPref(extension_id, kPrefAppNotificationDisbaled,
                      Value::CreateBooleanValue(value));
}
