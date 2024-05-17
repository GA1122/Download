void AppCacheUpdateJob::FetchManifest(bool is_first_fetch) {
  DCHECK(!manifest_fetcher_);
  manifest_fetcher_ =
      new URLFetcher(manifest_url_,
                     is_first_fetch ? URLFetcher::MANIFEST_FETCH
                                    : URLFetcher::MANIFEST_REFETCH,
                     this, kAppCacheFetchBufferSize);

  if (is_first_fetch) {
    AppCacheEntry* entry =
        (update_type_ == UPGRADE_ATTEMPT)
            ? group_->newest_complete_cache()->GetEntry(manifest_url_)
            : nullptr;
    if (entry && !doing_full_update_check_) {
      storage_->LoadResponseInfo(manifest_url_, entry->response_id(), this);
      return;
    }
    manifest_fetcher_->Start();
    return;
  }

  DCHECK(internal_state_ == REFETCH_MANIFEST);
  DCHECK(manifest_response_info_.get());
  manifest_fetcher_->set_existing_response_headers(
      manifest_response_info_->headers.get());
  manifest_fetcher_->Start();
}
