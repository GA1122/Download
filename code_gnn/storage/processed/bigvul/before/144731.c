 LocalSiteCharacteristicsDataImpl::LocalSiteCharacteristicsDataImpl(
     const url::Origin& origin,
     OnDestroyDelegate* delegate,
    LocalSiteCharacteristicsDatabase* database)
    : origin_(origin),
      loaded_tabs_count_(0U),
      loaded_tabs_in_background_count_(0U),
      database_(database),
      delegate_(delegate),
      safe_to_write_to_db_(false),
      is_dirty_(false),
      weak_factory_(this) {
  DCHECK(database_);
  DCHECK(delegate_);
  DCHECK(!site_characteristics_.IsInitialized());

  database_->ReadSiteCharacteristicsFromDB(
      origin_, base::BindOnce(&LocalSiteCharacteristicsDataImpl::OnInitCallback,
                              weak_factory_.GetWeakPtr()));
}
