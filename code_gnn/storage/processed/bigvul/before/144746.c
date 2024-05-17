LocalSiteCharacteristicsDataImpl::~LocalSiteCharacteristicsDataImpl() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  DCHECK(!IsLoaded());
  DCHECK_EQ(0U, loaded_tabs_in_background_count_);

  DCHECK(delegate_);
  delegate_->OnLocalSiteCharacteristicsDataImplDestroyed(this);

  if (is_dirty_ && safe_to_write_to_db_) {
    DCHECK(site_characteristics_.IsInitialized());
    database_->WriteSiteCharacteristicsIntoDB(origin_, site_characteristics_);
  }
}
