void LocalSiteCharacteristicsDataImpl::NotifyUpdatesFaviconInBackground() {
  NotifyFeatureUsage(
      site_characteristics_.mutable_updates_favicon_in_background());
}
