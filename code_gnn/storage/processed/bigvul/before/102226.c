void ExtensionPrefs::AcknowledgeOrphanedExtension(
    const std::string& extension_id) {
  DCHECK(Extension::IdIsValid(extension_id));
  UpdateExtensionPref(extension_id, kPrefOrphanAcknowledged,
                      Value::CreateBooleanValue(true));
}
