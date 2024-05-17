void ExtensionPrefs::SetAppLaunchOrdinal(const std::string& extension_id,
                                         const StringOrdinal& ordinal) {
  UpdateExtensionPref(extension_id, kPrefAppLaunchOrdinal,
                      Value::CreateStringValue(ordinal.ToString()));
}
