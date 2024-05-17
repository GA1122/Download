void ResourceFetcher::willStartLoadingResource(Resource* resource, ResourceRequest& request)
{
    if (m_documentLoader)
        m_documentLoader->applicationCacheHost()->willStartLoadingResource(request);

    storeResourceTimingInitiatorInformation(resource);
    TRACE_EVENT_ASYNC_BEGIN2("net", "Resource", resource, "url", resource->url().string().ascii(), "priority", resource->resourceRequest().priority());
}
