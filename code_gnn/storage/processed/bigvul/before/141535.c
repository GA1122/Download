bool ChooserContextBase::CanRequestObjectPermission(
    const GURL& requesting_origin,
    const GURL& embedding_origin) {
  ContentSetting content_setting =
      host_content_settings_map_->GetContentSetting(
          requesting_origin, embedding_origin, guard_content_settings_type_,
          std::string());
  DCHECK(content_setting == CONTENT_SETTING_ASK ||
         content_setting == CONTENT_SETTING_BLOCK);
  return content_setting == CONTENT_SETTING_ASK;
}
