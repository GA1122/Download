void LocalSiteCharacteristicsDataImpl::NotifyUsesAudioInBackground() {
  NotifyFeatureUsage(site_characteristics_.mutable_uses_audio_in_background());
}
