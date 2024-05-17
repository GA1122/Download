TabSpecificContentSettings::~TabSpecificContentSettings() {
  FOR_EACH_OBSERVER(
      SiteDataObserver, observer_list_, ContentSettingsDestroyed());
}
