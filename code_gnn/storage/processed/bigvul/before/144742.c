SiteFeatureUsage LocalSiteCharacteristicsDataImpl::UpdatesFaviconInBackground()
    const {
  return GetFeatureUsage(
      site_characteristics_.updates_favicon_in_background(),
      GetSiteCharacteristicsDatabaseParams().favicon_update_observation_window);
}
