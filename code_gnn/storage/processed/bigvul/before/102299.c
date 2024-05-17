int ExtensionPrefs::PageStringOrdinalAsInteger(
    const StringOrdinal& page_ordinal) const {
  if (!page_ordinal.IsValid())
    return -1;

  PageOrdinalMap::const_iterator it = page_ordinal_map_.find(page_ordinal);
  if (it != page_ordinal_map_.end()) {
    return std::distance(page_ordinal_map_.begin(), it);
  } else {
    return -1;
  }
}
