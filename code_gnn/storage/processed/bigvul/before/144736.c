void LocalSiteCharacteristicsDataImpl::NotifySiteUnloaded(
    TabVisibility tab_visibility) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (tab_visibility == TabVisibility::kBackground)
    DecrementNumLoadedBackgroundTabs();

  loaded_tabs_count_--;
  if (loaded_tabs_count_ > 0U)
    return;

  base::TimeDelta current_unix_time = GetTickDeltaSinceEpoch();

  site_characteristics_.set_last_loaded(
      TimeDeltaToInternalRepresentation(current_unix_time));
}
