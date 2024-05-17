void ResourceFetcher::InitializeRevalidation(
    ResourceRequest& revalidating_request,
    Resource* resource) {
  DCHECK(resource);
  DCHECK(GetMemoryCache()->Contains(resource));
  DCHECK(resource->IsLoaded());
  DCHECK(resource->CanUseCacheValidator());
  DCHECK(!resource->IsCacheValidator());
  DCHECK(!Context().IsControlledByServiceWorker());

  const AtomicString& last_modified =
      resource->GetResponse().HttpHeaderField(HTTPNames::Last_Modified);
  const AtomicString& e_tag =
      resource->GetResponse().HttpHeaderField(HTTPNames::ETag);
  if (!last_modified.IsEmpty() || !e_tag.IsEmpty()) {
    DCHECK_NE(WebCachePolicy::kBypassingCache,
              revalidating_request.GetCachePolicy());
    if (revalidating_request.GetCachePolicy() ==
        WebCachePolicy::kValidatingCacheData) {
      revalidating_request.SetHTTPHeaderField(HTTPNames::Cache_Control,
                                              "max-age=0");
    }
  }
  if (!last_modified.IsEmpty()) {
    revalidating_request.SetHTTPHeaderField(HTTPNames::If_Modified_Since,
                                            last_modified);
  }
  if (!e_tag.IsEmpty())
    revalidating_request.SetHTTPHeaderField(HTTPNames::If_None_Match, e_tag);

  resource->SetRevalidatingRequest(revalidating_request);
}
