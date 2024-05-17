void ResourceFetcher::notifyLoadedFromMemoryCache(Resource* resource)
{
    if (!frame() || !frame()->page() || resource->status() != Resource::Cached || m_validatedURLs.contains(resource->url()))
        return;

    ResourceRequest request(resource->url());
    unsigned long identifier = createUniqueIdentifier();
    context().dispatchDidLoadResourceFromMemoryCache(request, resource->response());
    willSendRequest(identifier, request, ResourceResponse(), resource->options().initiatorInfo);
    InspectorInstrumentation::markResourceAsCached(frame()->page(), identifier);
    context().sendRemainingDelegateMessages(m_documentLoader, identifier, resource->response(), resource->encodedSize());
}
