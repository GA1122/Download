void DocumentLoader::NotifyFinished(Resource* resource) {
  DCHECK_EQ(GetResource(), resource);
  DCHECK(GetResource());

  if (!resource->ErrorOccurred() && !resource->WasCanceled()) {
    FinishedLoading(resource->LoadFinishTime());
    return;
  }

  if (application_cache_host_)
    application_cache_host_->FailedLoadingMainResource();

  if (resource->GetResourceError().WasBlockedByResponse()) {
    probe::didReceiveResourceResponse(frame_->GetDocument(),
                                      MainResourceIdentifier(), this,
                                      resource->GetResponse(), resource);
  }

  LoadFailed(resource->GetResourceError());
  ClearResource();
}
