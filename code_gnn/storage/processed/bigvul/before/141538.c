std::unique_ptr<base::DictionaryValue> ChooserContextBase::GetWebsiteSetting(
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    content_settings::SettingInfo* info) {
  std::unique_ptr<base::DictionaryValue> value =
      base::DictionaryValue::From(host_content_settings_map_->GetWebsiteSetting(
          requesting_origin, embedding_origin, data_content_settings_type_,
          std::string(), info));
  if (!value)
    value.reset(new base::DictionaryValue());

  return value;
}
