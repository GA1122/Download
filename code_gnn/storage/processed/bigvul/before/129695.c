void ResourceFetcher::didFailLoading(const Resource* resource, const ResourceError& error)
{
    TRACE_EVENT_ASYNC_END0("net", "Resource", resource);
    context().dispatchDidFail(m_documentLoader, resource->identifier(), error);
}
