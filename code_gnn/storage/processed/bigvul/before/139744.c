Resource* ResourceFetcher::ResourceForBlockedRequest(
    const FetchRequest& request,
    const ResourceFactory& factory,
    ResourceRequestBlockedReason blocked_reason) {
  Resource* resource = factory.Create(request.GetResourceRequest(),
                                      request.Options(), request.Charset());
  resource->GetError(ResourceError::CancelledDueToAccessCheckError(
      request.Url(), blocked_reason));
  return resource;
}
