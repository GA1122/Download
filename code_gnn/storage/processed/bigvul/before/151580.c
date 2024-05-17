Resource* ResourceFetcher::ResourceForBlockedRequest(
    const FetchParameters& params,
    const ResourceFactory& factory,
    ResourceRequestBlockedReason blocked_reason) {
  Resource* resource = factory.Create(
      params.GetResourceRequest(), params.Options(), params.DecoderOptions());
  resource->SetStatus(ResourceStatus::kPending);
  resource->NotifyStartLoad();
  resource->FinishAsError(ResourceError::CancelledDueToAccessCheckError(
      params.Url(), blocked_reason));
  return resource;
}
