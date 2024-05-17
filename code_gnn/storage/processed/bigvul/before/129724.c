void ResourceFetcher::preCacheDataURIImage(const FetchRequest& request)
{
    const KURL& url = request.resourceRequest().url();
    ASSERT(url.protocolIsData());

    if (memoryCache()->resourceForURL(url))
        return;

    if (Resource* resource = resourceFromDataURIRequest(request.resourceRequest(), request.options())) {
        memoryCache()->add(resource);
        scheduleDocumentResourcesGC();
    }
}
