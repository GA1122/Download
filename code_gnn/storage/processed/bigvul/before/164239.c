void AppCacheUpdateJob::CheckIfManifestChanged() {
  DCHECK(update_type_ == UPGRADE_ATTEMPT);
  AppCacheEntry* entry = nullptr;
  if (group_->newest_complete_cache())
    entry = group_->newest_complete_cache()->GetEntry(manifest_url_);
  if (!entry) {
    if (service_->storage() == storage_) {
      AppCacheServiceImpl* service = service_;
      HandleCacheFailure(
          blink::mojom::AppCacheErrorDetails(
              "Manifest entry not found in existing cache",
              blink::mojom::AppCacheErrorReason::APPCACHE_UNKNOWN_ERROR, GURL(),
              0, false  ),
          DB_ERROR, GURL());
      AppCacheHistograms::AddMissingManifestEntrySample();
      service->DeleteAppCacheGroup(manifest_url_,
                                   net::CompletionOnceCallback());
    }
    return;
  }

  manifest_response_reader_ =
      storage_->CreateResponseReader(manifest_url_, entry->response_id());
  read_manifest_buffer_ =
      base::MakeRefCounted<net::IOBuffer>(kAppCacheFetchBufferSize);
  manifest_response_reader_->ReadData(
      read_manifest_buffer_.get(), kAppCacheFetchBufferSize,
      base::BindOnce(&AppCacheUpdateJob::OnManifestDataReadComplete,
                     base::Unretained(this)));   
}
