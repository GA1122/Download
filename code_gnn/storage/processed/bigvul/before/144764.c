void LocalSiteCharacteristicsWebContentsObserver::TitleWasSet(
    content::NavigationEntry* entry) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!first_time_title_set_) {
    first_time_title_set_ = true;
    return;
  }

  MaybeNotifyBackgroundFeatureUsage(
      &SiteCharacteristicsDataWriter::NotifyUpdatesTitleInBackground);
}
