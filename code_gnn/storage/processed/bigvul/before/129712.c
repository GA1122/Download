ResourcePtr<CSSStyleSheetResource> ResourceFetcher::fetchUserCSSStyleSheet(FetchRequest& request)
{
    KURL url = MemoryCache::removeFragmentIdentifierIfNeeded(request.resourceRequest().url());

    if (Resource* existing = memoryCache()->resourceForURL(url)) {
        if (existing->type() == Resource::CSSStyleSheet)
            return toCSSStyleSheetResource(existing);
        memoryCache()->remove(existing);
    }

    request.setOptions(ResourceLoaderOptions(SniffContent, BufferData, AllowStoredCredentials, ClientRequestedCredentials, CheckContentSecurityPolicy, DocumentContext));
    return toCSSStyleSheetResource(requestResource(Resource::CSSStyleSheet, request));
}
