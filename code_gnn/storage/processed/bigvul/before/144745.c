LocalSiteCharacteristicsDataImpl::UsesNotificationsInBackground() const {
  return GetFeatureUsage(
      site_characteristics_.uses_notifications_in_background(),
      GetSiteCharacteristicsDatabaseParams()
          .notifications_usage_observation_window);
}
