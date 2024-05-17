ChooserContextBase::ChooserContextBase(
    Profile* profile,
    const ContentSettingsType guard_content_settings_type,
    const ContentSettingsType data_content_settings_type)
    : guard_content_settings_type_(guard_content_settings_type),
      data_content_settings_type_(data_content_settings_type),
      host_content_settings_map_(
          HostContentSettingsMapFactory::GetForProfile(profile)) {
  DCHECK(host_content_settings_map_);
}
