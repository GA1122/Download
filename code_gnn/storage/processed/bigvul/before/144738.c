void LocalSiteCharacteristicsDataImpl::NotifyUpdatesTitleInBackground() {
  NotifyFeatureUsage(
      site_characteristics_.mutable_updates_title_in_background());
}
