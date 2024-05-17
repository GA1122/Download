void DocumentLoader::NotifyFinished(Resource* resource) {
  DCHECK_EQ(GetResource(), resource);
  DCHECK(GetResource());

  if (!resource->ErrorOccurred() && !resource->WasCanceled()) {
    FinishedLoading(TimeTicksFromSeconds(resource->LoadFinishTime()));
    return;
  }

  if (application_cache_host_)
    application_cache_host_->FailedLoadingMainResource();

  if (resource->GetResourceError().WasBlockedByResponse()) {
    probe::CanceledAfterReceivedResourceResponse(
        frame_, this, MainResourceIdentifier(), resource->GetResponse(),
        resource);
  }

  LoadFailed(resource->GetResourceError());
  ClearResource();
}
