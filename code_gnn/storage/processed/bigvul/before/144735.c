void LocalSiteCharacteristicsDataImpl::NotifySiteLoaded() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (loaded_tabs_count_ == 0) {
    site_characteristics_.set_last_loaded(
        TimeDeltaToInternalRepresentation(GetTickDeltaSinceEpoch()));

    is_dirty_ = true;
  }
  loaded_tabs_count_++;
}
