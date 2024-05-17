Resource* InspectorPageAgent::cachedResource(Frame* frame, const KURL& url)
{
    Resource* cachedResource = frame->document()->fetcher()->cachedResource(url);
    if (!cachedResource)
        cachedResource = memoryCache()->resourceForURL(url);
    return cachedResource;
}
