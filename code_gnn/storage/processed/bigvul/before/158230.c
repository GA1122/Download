AppCacheGroup::~AppCacheGroup() {
  DCHECK(old_caches_.empty());
  DCHECK(!newest_complete_cache_);
  DCHECK(restart_update_task_.IsCancelled());
  DCHECK(queued_updates_.empty());

  is_in_dtor_ = true;

  if (update_job_)
    delete update_job_;
  DCHECK_EQ(IDLE, update_status_);

  storage_->working_set()->RemoveGroup(this);
  storage_->DeleteResponses(manifest_url_, newly_deletable_response_ids_);
}
