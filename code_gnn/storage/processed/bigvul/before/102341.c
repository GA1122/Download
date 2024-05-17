void ExtensionPrefs::UpdatePageOrdinalMap(const StringOrdinal& old_value,
                                          const StringOrdinal& new_value) {
  if (new_value.IsValid())
    ++page_ordinal_map_[new_value];

  if (old_value.IsValid()) {
    --page_ordinal_map_[old_value];

    if (page_ordinal_map_[old_value] == 0)
      page_ordinal_map_.erase(old_value);
  }
}
