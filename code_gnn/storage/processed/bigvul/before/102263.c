StringOrdinal ExtensionPrefs::GetMinOrMaxAppLaunchOrdinalsOnPage(
    const StringOrdinal& target_page_ordinal,
    AppLaunchOrdinalReturn return_type) const {
  const DictionaryValue* extensions = prefs_->GetDictionary(kExtensionsPref);
  CHECK(extensions);
  CHECK(target_page_ordinal.IsValid());

  StringOrdinal return_value;
  for (DictionaryValue::key_iterator ext_it = extensions->begin_keys();
       ext_it != extensions->end_keys(); ++ext_it) {
    const StringOrdinal& page_ordinal = GetPageOrdinal(*ext_it);
    if (page_ordinal.IsValid() && page_ordinal.Equal(target_page_ordinal)) {
      const StringOrdinal& app_launch_ordinal = GetAppLaunchOrdinal(*ext_it);
      if (app_launch_ordinal.IsValid()) {
        if (!return_value.IsValid())
          return_value = app_launch_ordinal;
        else if (return_type == ExtensionPrefs::MIN_ORDINAL &&
                 app_launch_ordinal.LessThan(return_value))
          return_value = app_launch_ordinal;
        else if (return_type == ExtensionPrefs::MAX_ORDINAL &&
                 app_launch_ordinal.GreaterThan(return_value))
          return_value = app_launch_ordinal;
      }
    }
  }

  return return_value;
}
