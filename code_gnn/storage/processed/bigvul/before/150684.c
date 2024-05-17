 void PageInfo::PresentSitePermissions() {
  PermissionInfoList permission_info_list;
  ChosenObjectInfoList chosen_object_info_list;

  PageInfoUI::PermissionInfo permission_info;
  for (size_t i = 0; i < base::size(kPermissionType); ++i) {
    permission_info.type = kPermissionType[i];

    content_settings::SettingInfo info;
    std::unique_ptr<base::Value> value = content_settings_->GetWebsiteSetting(
        site_url_, site_url_, permission_info.type, std::string(), &info);
    DCHECK(value.get());
    if (value->type() == base::Value::Type::INTEGER) {
      permission_info.setting =
          content_settings::ValueToContentSetting(value.get());
    } else {
      NOTREACHED();
    }

    permission_info.source = info.source;
    permission_info.is_incognito = profile_->IsOffTheRecord();

    if (info.primary_pattern == ContentSettingsPattern::Wildcard() &&
        info.secondary_pattern == ContentSettingsPattern::Wildcard()) {
      permission_info.default_setting = permission_info.setting;
      permission_info.setting = CONTENT_SETTING_DEFAULT;
    } else {
      permission_info.default_setting =
          content_settings_->GetDefaultContentSetting(permission_info.type,
                                                      NULL);
    }

    if (PermissionUtil::IsPermission(permission_info.type) &&
        permission_info.setting == CONTENT_SETTING_DEFAULT &&
        permission_info.source ==
            content_settings::SettingSource::SETTING_SOURCE_USER) {
      PermissionResult permission_result =
          PermissionManager::Get(profile_)->GetPermissionStatus(
              permission_info.type, site_url_, site_url_);

      if (permission_result.content_setting == CONTENT_SETTING_BLOCK &&
          permission_result.source ==
              PermissionStatusSource::MULTIPLE_DISMISSALS) {
        permission_info.setting = permission_result.content_setting;
      }
    }

    if (ShouldShowPermission(permission_info, site_url_, content_settings_,
                             web_contents(), tab_specific_content_settings())) {
      permission_info_list.push_back(permission_info);
    }
  }

  const auto origin = url::Origin::Create(site_url_);
  for (const ChooserUIInfo& ui_info : kChooserUIInfo) {
    ChooserContextBase* context = ui_info.get_context(profile_);
    auto chosen_objects = context->GetGrantedObjects(origin, origin);
    for (std::unique_ptr<ChooserContextBase::Object>& object : chosen_objects) {
      chosen_object_info_list.push_back(
          std::make_unique<PageInfoUI::ChosenObjectInfo>(ui_info,
                                                         std::move(object)));
    }
  }

  ui_->SetPermissionInfo(permission_info_list,
                         std::move(chosen_object_info_list));
}
