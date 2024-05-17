void AppCacheHost::FinishCacheSelection(
    AppCache *cache, AppCacheGroup* group) {
  DCHECK(!associated_cache());

  if (cache) {
    DCHECK(cache->owning_group());
    DCHECK(new_master_entry_url_.is_empty());
    DCHECK_EQ(cache->owning_group()->manifest_url(), preferred_manifest_url_);
    AppCacheGroup* owing_group = cache->owning_group();
    const char* kFormatString =
        "Document was loaded from Application Cache with manifest %s";
    frontend_->OnLogMessage(
        host_id_, APPCACHE_LOG_INFO,
        base::StringPrintf(
            kFormatString, owing_group->manifest_url().spec().c_str()));
    AssociateCompleteCache(cache);
    if (!owing_group->is_obsolete() && !owing_group->is_being_deleted()) {
      owing_group->StartUpdateWithHost(this);
      ObserveGroupBeingUpdated(owing_group);
    }
  } else if (group && !group->is_being_deleted()) {
    DCHECK(!group->is_obsolete());
    DCHECK(new_master_entry_url_.is_valid());
    DCHECK_EQ(group->manifest_url(), preferred_manifest_url_);
    const char* kFormatString = group->HasCache() ?
        "Adding master entry to Application Cache with manifest %s" :
        "Creating Application Cache with manifest %s";
    frontend_->OnLogMessage(
        host_id_, APPCACHE_LOG_INFO,
        base::StringPrintf(kFormatString,
                           group->manifest_url().spec().c_str()));
    AssociateNoCache(preferred_manifest_url_);
    group->StartUpdateWithNewMasterEntry(this, new_master_entry_url_);
    ObserveGroupBeingUpdated(group);
  } else {
    new_master_entry_url_ = GURL();
    AssociateNoCache(GURL());
  }

  if (!pending_get_status_callback_.is_null())
    DoPendingGetStatus();
  else if (!pending_start_update_callback_.is_null())
    DoPendingStartUpdate();
  else if (!pending_swap_cache_callback_.is_null())
    DoPendingSwapCache();

  FOR_EACH_OBSERVER(Observer, observers_, OnCacheSelectionComplete(this));
}
