void AppCacheUpdateJob::HandleManifestRefetchCompleted(URLFetcher* fetcher,
                                                       int net_error) {
  DCHECK(internal_state_ == REFETCH_MANIFEST);
  DCHECK(manifest_fetcher_ == fetcher);
  manifest_fetcher_ = nullptr;

  int response_code =
      net_error == net::OK ? fetcher->request()->GetResponseCode() : -1;
  if (response_code == 304 || manifest_data_ == fetcher->manifest_data()) {
    AppCacheEntry* entry = nullptr;
    if (inprogress_cache_)
      entry = inprogress_cache_->GetEntry(manifest_url_);
    if (entry) {
      entry->add_types(AppCacheEntry::MANIFEST);
      StoreGroupAndCache();
    } else {
      manifest_response_writer_ = CreateResponseWriter();
      scoped_refptr<HttpResponseInfoIOBuffer> io_buffer =
          base::MakeRefCounted<HttpResponseInfoIOBuffer>(
              std::move(manifest_response_info_));
      manifest_response_writer_->WriteInfo(
          io_buffer.get(),
          base::BindOnce(&AppCacheUpdateJob::OnManifestInfoWriteComplete,
                         base::Unretained(this)));
    }
  } else {
    VLOG(1) << "Request error: " << net_error
            << " response code: " << response_code;
    ScheduleUpdateRetry(kRerunDelayMs);
    if (response_code == 200) {
      HandleCacheFailure(
          blink::mojom::AppCacheErrorDetails(
              "Manifest changed during update",
              blink::mojom::AppCacheErrorReason::APPCACHE_CHANGED_ERROR, GURL(),
              0, false  ),
          MANIFEST_ERROR, GURL());
    } else {
      const char kFormatString[] = "Manifest re-fetch failed (%d) %s";
      std::string message = FormatUrlErrorMessage(
          kFormatString, manifest_url_, fetcher->result(), response_code);
      ResultType result = fetcher->result();
      if (result == UPDATE_OK) {
        result = SERVER_ERROR;
      }
      HandleCacheFailure(
          blink::mojom::AppCacheErrorDetails(
              message,
              blink::mojom::AppCacheErrorReason::APPCACHE_MANIFEST_ERROR,
              GURL(), response_code, false  ),
          result, GURL());
    }
  }
}
