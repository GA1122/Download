void ExtensionPrefs::MigrateAppIndex(const ExtensionIdSet& extension_ids) {
  if (extension_ids.empty())
    return;

  typedef std::map<StringOrdinal, std::map<int, const std::string*>,
      StringOrdinalLessThan> AppPositionToIdMapping;
  AppPositionToIdMapping app_launches_to_convert;
  for (ExtensionIdSet::const_iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    int old_page_index = 0;
    StringOrdinal page = GetPageOrdinal(*ext_id);
    if (ReadExtensionPrefInteger(*ext_id,
                                 kPrefPageIndexDeprecated,
                                 &old_page_index)) {
      if (old_page_index < 0) {
        DLOG(WARNING) << "Extension " << *ext_id
                      << " has an invalid page index " << old_page_index
                      << ". Aborting attempt to convert its index.";
        break;
      }

      if (page_ordinal_map_.empty())
        page_ordinal_map_[StringOrdinal::CreateInitialOrdinal()] = 0;
      while (page_ordinal_map_.size() <= static_cast<size_t>(old_page_index)) {
        StringOrdinal earlier_page =
            page_ordinal_map_.rbegin()->first.CreateAfter();
        page_ordinal_map_[earlier_page] = 0;
      }

      page = PageIntegerAsStringOrdinal(old_page_index);
      SetPageOrdinal(*ext_id, page);
      UpdateExtensionPref(*ext_id, kPrefPageIndexDeprecated, NULL);
    }

    int old_app_launch_index = 0;
    if (ReadExtensionPrefInteger(*ext_id,
                                 kPrefAppLaunchIndexDeprecated,
                                 &old_app_launch_index)) {
      if (page.IsValid())
        app_launches_to_convert[page][old_app_launch_index] = &*ext_id;

      UpdateExtensionPref(*ext_id, kPrefAppLaunchIndexDeprecated, NULL);
    }
  }

  for (PageOrdinalMap::iterator it = page_ordinal_map_.begin();
       it != page_ordinal_map_.end();) {
    if (it->second == 0) {
      PageOrdinalMap::iterator prev_it = it;
      ++it;
      page_ordinal_map_.erase(prev_it);
    } else {
      ++it;
    }
  }

  if (app_launches_to_convert.empty())
    return;

  for (AppPositionToIdMapping::const_iterator page_it =
           app_launches_to_convert.begin();
       page_it != app_launches_to_convert.end(); ++page_it) {
    StringOrdinal page = page_it->first;
    for (std::map<int, const std::string*>::const_iterator launch_it =
            page_it->second.begin(); launch_it != page_it->second.end();
        ++launch_it) {
      SetAppLaunchOrdinal(*(launch_it->second),
                          CreateNextAppLaunchOrdinal(page));
    }
  }
}
