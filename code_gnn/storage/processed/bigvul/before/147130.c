void DocumentLoader::NotifyFinished(Resource* resource) {
  DCHECK_EQ(main_resource_, resource);
  DCHECK(main_resource_);

  if (!main_resource_->ErrorOccurred() && !main_resource_->WasCanceled()) {
    FinishedLoading(main_resource_->LoadFinishTime());
    return;
  }

  if (application_cache_host_)
    application_cache_host_->FailedLoadingMainResource();

  if (main_resource_->GetResourceError().WasBlockedByResponse()) {
    probe::CanceledAfterReceivedResourceResponse(
        frame_, this, MainResourceIdentifier(), resource->GetResponse(),
        main_resource_.Get());
  }

  LoadFailed(main_resource_->GetResourceError());
  ClearMainResourceHandle();
}
