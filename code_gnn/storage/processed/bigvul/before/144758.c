void LocalSiteCharacteristicsWebContentsObserver::OnAudioStateChanged(
    bool audible) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!audible)
    return;

  MaybeNotifyBackgroundFeatureUsage(
      &SiteCharacteristicsDataWriter::NotifyUsesAudioInBackground);
}
