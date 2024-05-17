void ExtensionPrefs::InitializePageOrdinalMap(
    const ExtensionIdSet& extension_ids) {
  for (ExtensionIdSet::const_iterator ext_it = extension_ids.begin();
       ext_it != extension_ids.end(); ++ext_it) {
    UpdatePageOrdinalMap(StringOrdinal(), GetPageOrdinal(*ext_it));
  }
}
