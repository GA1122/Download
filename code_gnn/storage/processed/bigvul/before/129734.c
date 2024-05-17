void ResourceFetcher::requestPreload(Resource::Type type, FetchRequest& request, const String& charset)
{
    if (type == Resource::MainResource)
        return;

    String encoding;
    if (type == Resource::Script || type == Resource::CSSStyleSheet)
        encoding = charset.isEmpty() ? m_document->charset().string() : charset;

    request.setCharset(encoding);
    request.setForPreload(true);

    ResourcePtr<Resource> resource = requestResource(type, request);
    if (!resource || (m_preloads && m_preloads->contains(resource.get())))
        return;
    TRACE_EVENT_ASYNC_STEP_INTO0("net", "Resource", resource.get(), "Preload");
    resource->increasePreloadCount();

    if (!m_preloads)
        m_preloads = adoptPtr(new ListHashSet<Resource*>);
    m_preloads->add(resource.get());

#if PRELOAD_DEBUG
    printf("PRELOADING %s\n",  resource->url().string().latin1().data());
#endif
}
