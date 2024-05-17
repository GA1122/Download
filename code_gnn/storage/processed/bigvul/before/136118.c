void WebsiteSettings::PresentSitePermissions() {
  PermissionInfoList permission_info_list;

  WebsiteSettingsUI::PermissionInfo permission_info;
  for (size_t i = 0; i < arraysize(kPermissionType); ++i) {
    permission_info.type = kPermissionType[i];

    content_settings::SettingInfo info;
    scoped_ptr<base::Value> value =
        content_settings_->GetWebsiteSetting(
            site_url_, site_url_, permission_info.type, std::string(), &info);
    DCHECK(value.get());
    if (value->GetType() == base::Value::TYPE_INTEGER) {
      permission_info.setting =
          content_settings::ValueToContentSetting(value.get());
    } else {
      NOTREACHED();
    }

    permission_info.source = info.source;

    if (info.primary_pattern == ContentSettingsPattern::Wildcard() &&
        info.secondary_pattern == ContentSettingsPattern::Wildcard()) {
      permission_info.default_setting = permission_info.setting;
      permission_info.setting = CONTENT_SETTING_DEFAULT;
    } else {
      permission_info.default_setting =
          content_settings_->GetDefaultContentSetting(permission_info.type,
                                                      NULL);
    }

    if (permission_info.setting != CONTENT_SETTING_DEFAULT &&
        permission_info.setting != permission_info.default_setting) {
      permission_info_list.push_back(permission_info);
    }
  }

  ui_->SetPermissionInfo(permission_info_list);
}
