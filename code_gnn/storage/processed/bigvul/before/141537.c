ChooserContextBase::GetAllGrantedObjects() {
  ContentSettingsForOneType content_settings;
  host_content_settings_map_->GetSettingsForOneType(
      data_content_settings_type_, std::string(), &content_settings);

  std::vector<std::unique_ptr<Object>> results;
  for (const ContentSettingPatternSource& content_setting : content_settings) {
    GURL requesting_origin(content_setting.primary_pattern.ToString());
    GURL embedding_origin(content_setting.secondary_pattern.ToString());
    if (!requesting_origin.is_valid() || !embedding_origin.is_valid())
      continue;

    if (!CanRequestObjectPermission(requesting_origin, embedding_origin))
      continue;

    content_settings::SettingInfo info;
    std::unique_ptr<base::DictionaryValue> setting =
        GetWebsiteSetting(requesting_origin, embedding_origin, &info);
    base::ListValue* object_list;
    if (!setting->GetList(kObjectListKey, &object_list))
      continue;

    for (auto& object : *object_list) {
      base::DictionaryValue* object_dict;
      if (!object.GetAsDictionary(&object_dict) ||
          !IsValidObject(*object_dict)) {
        continue;
      }

      results.push_back(std::make_unique<Object>(
          requesting_origin, embedding_origin, object_dict, info.source,
          content_setting.incognito));
    }
  }

  return results;
}
