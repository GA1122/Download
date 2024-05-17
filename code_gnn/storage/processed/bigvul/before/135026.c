void AppCacheUpdateJob::URLFetcher::OnResponseCompleted() {
  if (request_->status().is_success())
    job_->MadeProgress();

  if (request_->status().is_success() &&
      request_->GetResponseCode() == 503 &&
      MaybeRetryRequest()) {
    return;
  }

  switch (fetch_type_) {
    case MANIFEST_FETCH:
      job_->HandleManifestFetchCompleted(this);
      break;
    case URL_FETCH:
      job_->HandleUrlFetchCompleted(this);
      break;
    case MASTER_ENTRY_FETCH:
      job_->HandleMasterEntryFetchCompleted(this);
      break;
    case MANIFEST_REFETCH:
      job_->HandleManifestRefetchCompleted(this);
      break;
    default:
      NOTREACHED();
  }

  delete this;
}
