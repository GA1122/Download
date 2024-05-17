void LocalSiteCharacteristicsDataImpl::NotifyLoadedSiteBackgrounded() {
  if (loaded_tabs_in_background_count_ == 0)
    background_session_begin_ = NowTicks();

  loaded_tabs_in_background_count_++;

  DCHECK_LE(loaded_tabs_in_background_count_, loaded_tabs_count_);
}
