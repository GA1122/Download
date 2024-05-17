bool IsPermissionFactoryDefault(HostContentSettingsMap* content_settings,
                                const PageInfoUI::PermissionInfo& info) {
  const ContentSetting factory_default_setting =
      content_settings::ContentSettingsRegistry::GetInstance()
          ->Get(info.type)
          ->GetInitialDefaultSetting();
  return (info.source == content_settings::SETTING_SOURCE_USER &&
          factory_default_setting == info.default_setting &&
          info.setting == CONTENT_SETTING_DEFAULT);
}
