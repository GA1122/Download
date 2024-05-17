void TabSpecificContentSettings::NotifySiteDataObservers() {
  FOR_EACH_OBSERVER(SiteDataObserver, observer_list_, OnSiteDataAccessed());
}
