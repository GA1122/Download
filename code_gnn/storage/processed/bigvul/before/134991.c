void AppCacheUpdateJob::CheckIfManifestChanged() {
  DCHECK(update_type_ == UPGRADE_ATTEMPT);
  AppCacheEntry* entry = NULL;
  if (group_->newest_complete_cache())
    entry = group_->newest_complete_cache()->GetEntry(manifest_url_);
  if (!entry) {
    if (service_->storage() == storage_) {
      AppCacheServiceImpl* service = service_;
      HandleCacheFailure(
          AppCacheErrorDetails("Manifest entry not found in existing cache",
                       APPCACHE_UNKNOWN_ERROR,
                       GURL(),
                       0,
                       false  ),
          DB_ERROR,
          GURL());
      AppCacheHistograms::AddMissingManifestEntrySample();
      service->DeleteAppCacheGroup(manifest_url_, net::CompletionCallback());
    }
    return;
  }

  manifest_response_reader_.reset(
      storage_->CreateResponseReader(manifest_url_,
                                     group_->group_id(),
                                     entry->response_id()));
  read_manifest_buffer_ = new net::IOBuffer(kBufferSize);
  manifest_response_reader_->ReadData(
      read_manifest_buffer_.get(),
      kBufferSize,
      base::Bind(&AppCacheUpdateJob::OnManifestDataReadComplete,
                 base::Unretained(this)));   
}
