void ResourceFetcher::preCacheSubstituteDataForMainResource(const FetchRequest& request, const SubstituteData& substituteData)
{
    const KURL& url = request.url();
    if (Resource* oldResource = memoryCache()->resourceForURL(url))
        memoryCache()->remove(oldResource);

    ResourceResponse response(url, substituteData.mimeType(), substituteData.content()->size(), substituteData.textEncoding(), emptyString());
    ResourcePtr<Resource> resource = createResource(Resource::MainResource, request.resourceRequest(), substituteData.textEncoding());
    resource->setNeedsSynchronousCacheHit(substituteData.forceSynchronousLoad());
    resource->setOptions(request.options());
    resource->setDataBufferingPolicy(BufferData);
    resource->responseReceived(response);
    if (substituteData.content()->size())
        resource->setResourceBuffer(substituteData.content());
    resource->finish();
    memoryCache()->add(resource.get());
}
