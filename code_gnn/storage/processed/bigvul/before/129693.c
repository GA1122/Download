void ResourceFetcher::didChangeLoadingPriority(const Resource* resource, ResourceLoadPriority loadPriority, int intraPriorityValue)
{
    TRACE_EVENT_ASYNC_STEP_INTO1("net", "Resource", resource, "ChangePriority", "priority", loadPriority);
    context().dispatchDidChangeResourcePriority(resource->identifier(), loadPriority, intraPriorityValue);
}
