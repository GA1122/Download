void ExtensionPrefs::SetPageOrdinal(const std::string& extension_id,
                                    const StringOrdinal& page_ordinal) {
  UpdatePageOrdinalMap(GetPageOrdinal(extension_id), page_ordinal);
  UpdateExtensionPref(extension_id, kPrefPageOrdinal,
                      Value::CreateStringValue(page_ordinal.ToString()));
}
