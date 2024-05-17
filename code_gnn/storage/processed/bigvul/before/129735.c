ResourcePtr<Resource> ResourceFetcher::requestResource(Resource::Type type, FetchRequest& request)
{
    ASSERT(request.options().synchronousPolicy == RequestAsynchronously || type == Resource::Raw);

    TRACE_EVENT0("webkit", "ResourceFetcher::requestResource");

    KURL url = request.resourceRequest().url();

    WTF_LOG(ResourceLoading, "ResourceFetcher::requestResource '%s', charset '%s', priority=%d, forPreload=%u, type=%s", url.elidedString().latin1().data(), request.charset().latin1().data(), request.priority(), request.forPreload(), ResourceTypeName(type));

    url = MemoryCache::removeFragmentIdentifierIfNeeded(url);

    if (!url.isValid())
        return 0;

    if (!canRequest(type, url, request.options(), request.forPreload(), request.originRestriction()))
        return 0;

    if (LocalFrame* f = frame())
        f->loader().client()->dispatchWillRequestResource(&request);

    ResourcePtr<Resource> resource = memoryCache()->resourceForURL(url);

    const RevalidationPolicy policy = determineRevalidationPolicy(type, request.mutableResourceRequest(), request.forPreload(), resource.get(), request.defer(), request.options());
    switch (policy) {
    case Reload:
        memoryCache()->remove(resource.get());
    case Load:
        resource = createResourceForLoading(type, request, request.charset());
        break;
    case Revalidate:
        resource = createResourceForRevalidation(request, resource.get());
        break;
    case Use:
        memoryCache()->updateForAccess(resource.get());
        break;
    }

    if (!resource)
        return 0;

    if (!resource->hasClients())
        m_deadStatsRecorder.update(policy);

    if (policy != Use)
        resource->setIdentifier(createUniqueIdentifier());

    if (!request.forPreload() || policy != Use) {
        ResourceLoadPriority priority = loadPriority(type, request);
        if (priority != resource->resourceRequest().priority()) {
            resource->resourceRequest().setPriority(priority);
            resource->didChangePriority(priority, 0);
        }
    }

    if (resourceNeedsLoad(resource.get(), request, policy)) {
        if (!shouldLoadNewResource(type)) {
            if (memoryCache()->contains(resource.get()))
                memoryCache()->remove(resource.get());
            return 0;
        }

        if (!m_documentLoader || !m_documentLoader->scheduleArchiveLoad(resource.get(), request.resourceRequest()))
            resource->load(this, request.options());

        if (resource->errorOccurred()) {
            if (memoryCache()->contains(resource.get()))
                memoryCache()->remove(resource.get());
            return request.options().synchronousPolicy == RequestSynchronously ? resource : 0;
        }
    }

    if (type == Resource::MainResource) {
        ASSERT(policy != Use || m_documentLoader->substituteData().isValid());
        ASSERT(policy != Revalidate);
        memoryCache()->remove(resource.get());
    }

    requestLoadStarted(resource.get(), request, policy == Use ? ResourceLoadingFromCache : ResourceLoadingFromNetwork);

    ASSERT(resource->url() == url.string());
    m_documentResources.set(resource->url(), resource);
    return resource;
}
