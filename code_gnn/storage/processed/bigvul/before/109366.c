void InspectorResourceAgent::didReceiveResourceResponse(unsigned long identifier, DocumentLoader* loader, const ResourceResponse& response, ResourceLoader* resourceLoader)
{
    if (!loader)
        return;

    String requestId = IdentifiersFactory::requestId(identifier);
    RefPtr<TypeBuilder::Network::Response> resourceResponse = buildObjectForResourceResponse(response, loader);

    bool isNotModified = response.httpStatusCode() == 304;

    Resource* cachedResource = 0;
    if (resourceLoader && !isNotModified)
        cachedResource = resourceLoader->cachedResource();
    if (!cachedResource || cachedResource->type() == Resource::MainResource)
        cachedResource = InspectorPageAgent::cachedResource(loader->frame(), response.url());

    if (cachedResource) {
        if (resourceResponse && response.mimeType().isEmpty())
            resourceResponse->setString(TypeBuilder::Network::Response::MimeType, cachedResource->response().mimeType());
        m_resourcesData->addResource(requestId, cachedResource);
    }

    InspectorPageAgent::ResourceType type = cachedResource ? InspectorPageAgent::cachedResourceType(*cachedResource) : InspectorPageAgent::OtherResource;
    if (m_resourcesData->resourceType(requestId) == InspectorPageAgent::XHRResource)
        type = InspectorPageAgent::XHRResource;
    else if (m_resourcesData->resourceType(requestId) == InspectorPageAgent::ScriptResource)
        type = InspectorPageAgent::ScriptResource;
    else if (equalIgnoringFragmentIdentifier(response.url(), loader->url()) && !loader->isCommitted())
        type = InspectorPageAgent::DocumentResource;

    m_resourcesData->responseReceived(requestId, m_pageAgent->frameId(loader->frame()), response);
    m_resourcesData->setResourceType(requestId, type);
    m_frontend->responseReceived(requestId, m_pageAgent->frameId(loader->frame()), m_pageAgent->loaderId(loader), currentTime(), InspectorPageAgent::resourceTypeJson(type), resourceResponse);
    if (isNotModified && cachedResource && cachedResource->encodedSize())
        didReceiveData(identifier, 0, cachedResource->encodedSize(), 0);
}
