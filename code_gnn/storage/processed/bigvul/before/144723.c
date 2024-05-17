void LocalSiteCharacteristicsDataImpl::DecrementNumLoadedBackgroundTabs() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  DCHECK_GT(loaded_tabs_in_background_count_, 0U);
  loaded_tabs_in_background_count_--;
  if (loaded_tabs_in_background_count_ > 0U)
    return;

  DCHECK(!background_session_begin_.is_null());
  base::TimeDelta extra_observation_duration =
      NowTicks() - background_session_begin_;

  for (auto* iter : GetAllFeaturesFromProto(&site_characteristics_))
    IncrementFeatureObservationDuration(iter, extra_observation_duration);
}
