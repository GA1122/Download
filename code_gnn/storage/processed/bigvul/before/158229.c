void AppCacheGroup::StartUpdateWithNewMasterEntry(
    AppCacheHost* host, const GURL& new_master_resource) {
  DCHECK(!is_obsolete() && !is_being_deleted());
  if (is_in_dtor_)
    return;

  if (!update_job_)
    update_job_ = new AppCacheUpdateJob(storage_->service(), this);

  update_job_->StartUpdate(host, new_master_resource);

  if (!restart_update_task_.IsCancelled()) {
    restart_update_task_.Cancel();
    RunQueuedUpdates();
  }
}
