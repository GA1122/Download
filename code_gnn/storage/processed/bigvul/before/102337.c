void ExtensionPrefs::SetUpdateUrlData(const std::string& extension_id,
                                      const std::string& data) {
  UpdateExtensionPref(extension_id, kUpdateUrlData,
                      Value::CreateStringValue(data));
}
