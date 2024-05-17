SiteFeatureUsage LocalSiteCharacteristicsDataImpl::UpdatesTitleInBackground()
    const {
  return GetFeatureUsage(
      site_characteristics_.updates_title_in_background(),
      GetSiteCharacteristicsDatabaseParams().title_update_observation_window);
}
