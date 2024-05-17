AppCacheGroup::AppCacheGroup(AppCacheStorage* storage,
                             const GURL& manifest_url,
                             int64_t group_id)
    : group_id_(group_id),
      manifest_url_(manifest_url),
      update_status_(IDLE),
      is_obsolete_(false),
      is_being_deleted_(false),
      newest_complete_cache_(nullptr),
      update_job_(nullptr),
      storage_(storage),
      is_in_dtor_(false) {
  storage_->working_set()->AddGroup(this);
  host_observer_.reset(new HostObserver(this));
}
