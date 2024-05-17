void AppCacheUpdateJob::MaybeCompleteUpdate() {
  DCHECK(internal_state_ != CACHE_FAILURE);

  if (master_entries_completed_ != pending_master_entries_.size() ||
      url_fetches_completed_ != url_file_list_.size()) {
    DCHECK(internal_state_ != COMPLETED);
    return;
  }

  switch (internal_state_) {
    case NO_UPDATE:
      if (master_entries_completed_ > 0) {
        switch (stored_state_) {
          case UNSTORED:
            StoreGroupAndCache();
            return;
          case STORING:
            return;
          case STORED:
            break;
        }
      } else {
        bool times_changed = false;
        if (!group_->first_evictable_error_time().is_null()) {
          group_->set_first_evictable_error_time(base::Time());
          times_changed = true;
        }
        if (doing_full_update_check_) {
          group_->set_last_full_update_check_time(base::Time::Now());
          times_changed = true;
        }
        if (times_changed)
          storage_->StoreEvictionTimes(group_);
      }
      group_->SetUpdateAppCacheStatus(AppCacheGroup::IDLE);
      NotifyAllAssociatedHosts(
          blink::mojom::AppCacheEventID::APPCACHE_NO_UPDATE_EVENT);
      DiscardDuplicateResponses();
      internal_state_ = COMPLETED;
      break;
    case DOWNLOADING:
      internal_state_ = REFETCH_MANIFEST;
      FetchManifest(false);
      break;
    case REFETCH_MANIFEST:
      DCHECK(stored_state_ == STORED);
      NotifyAllFinalProgress();
      group_->SetUpdateAppCacheStatus(AppCacheGroup::IDLE);
      if (update_type_ == CACHE_ATTEMPT)
        NotifyAllAssociatedHosts(
            blink::mojom::AppCacheEventID::APPCACHE_CACHED_EVENT);
      else
        NotifyAllAssociatedHosts(
            blink::mojom::AppCacheEventID::APPCACHE_UPDATE_READY_EVENT);
      DiscardDuplicateResponses();
      internal_state_ = COMPLETED;
      LogHistogramStats(UPDATE_OK, GURL());
      break;
    case CACHE_FAILURE:
      NOTREACHED();   
      break;
    default:
      break;
  }

  if (internal_state_ == COMPLETED)
    DeleteSoon();
}
