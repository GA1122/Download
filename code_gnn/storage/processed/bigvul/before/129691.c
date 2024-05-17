ResourceFetcher::RevalidationPolicy ResourceFetcher::determineRevalidationPolicy(Resource::Type type, ResourceRequest& request, bool forPreload, Resource* existingResource, FetchRequest::DeferOption defer, const ResourceLoaderOptions& options) const
{
    if (!existingResource)
        return Load;

    if (forPreload && existingResource->isPreloaded())
        return Use;

    if (existingResource->type() != type) {
        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicy reloading due to type mismatch.");
        return Reload;
    }

    if (FetchRequest::DeferredByClient == defer)
        return Reload;

    if (type == Resource::Image && request.url().protocolIsData())
        return Use;

    if (type == Resource::MainResource && m_documentLoader->substituteData().isValid())
        return Use;

    if (!existingResource->canReuse(request))
        return Reload;

    if (request.downloadToFile())
        return Reload;

    if (request.isConditional())
        return Reload;

    if (m_allowStaleResources)
        return Use;

    if (existingResource->isPreloaded())
        return Use;

    CachePolicy cachePolicy = context().cachePolicy(document());
    if (cachePolicy == CachePolicyHistoryBuffer)
        return Use;

    if (existingResource->hasCacheControlNoStoreHeader()) {
        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicy reloading due to Cache-control: no-store.");
        return Reload;
    }

    if (type != Resource::MainResource && options.corsEnabled != existingResource->options().corsEnabled)
        return Reload;

    if (existingResource->resourceRequest().allowStoredCredentials() != request.allowStoredCredentials()) {
        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicy reloading due to difference in credentials settings.");
        return Reload;
    }

    if (type != Resource::Raw) {
        if (document() && !document()->loadEventFinished() && m_validatedURLs.contains(existingResource->url()))
            return Use;
        if (existingResource->isLoading())
            return Use;
    }

    if (cachePolicy == CachePolicyReload) {
        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicy reloading due to CachePolicyReload.");
        return Reload;
    }

    if (existingResource->errorOccurred()) {
        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicye reloading due to resource being in the error state");
        return Reload;
    }

    if (type == Resource::Image && existingResource == cachedResource(request.url()))
        return Use;

    if (!existingResource->canReuseRedirectChain()) {
        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicy reloading due to an uncacheable redirect");
        return Reload;
    }

    if (cachePolicy == CachePolicyRevalidate || existingResource->mustRevalidateDueToCacheHeaders()
        || request.cacheControlContainsNoCache()) {
        if (existingResource->canUseCacheValidator())
            return Revalidate;

        WTF_LOG(ResourceLoading, "ResourceFetcher::determineRevalidationPolicy reloading due to missing cache validators.");
        return Reload;
    }

    return Use;
}
