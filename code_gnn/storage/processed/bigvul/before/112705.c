void DocumentLoader::startLoadingMainResource()
{
    m_mainDocumentError = ResourceError();
    timing()->markNavigationStart();
    ASSERT(!m_mainResource);
    ASSERT(!m_loadingMainResource);
    m_loadingMainResource = true;

    if (maybeLoadEmpty())
        return;

    frameLoader()->addExtraFieldsToMainResourceRequest(m_request);

    ASSERT(timing()->navigationStart());
    ASSERT(!timing()->fetchStart());
    timing()->markFetchStart();
    willSendRequest(m_request, ResourceResponse());

    if (!m_frame || m_request.isNull())
        return;

    m_applicationCacheHost->maybeLoadMainResource(m_request, m_substituteData);

    if (m_substituteData.isValid()) {
        m_identifierForLoadWithoutResourceLoader = m_frame->page()->progress()->createUniqueIdentifier();
        frameLoader()->notifier()->assignIdentifierToInitialRequest(m_identifierForLoadWithoutResourceLoader, this, m_request);
        frameLoader()->notifier()->dispatchWillSendRequest(this, m_identifierForLoadWithoutResourceLoader, m_request, ResourceResponse());
        handleSubstituteDataLoadSoon();
        return;
    }

    ResourceRequest request(m_request);
    DEFINE_STATIC_LOCAL(ResourceLoaderOptions, mainResourceLoadOptions,
        (SendCallbacks, SniffContent, BufferData, AllowStoredCredentials, AskClientForCrossOriginCredentials, SkipSecurityCheck));
    CachedResourceRequest cachedResourceRequest(request, mainResourceLoadOptions);
    m_mainResource = m_cachedResourceLoader->requestMainResource(cachedResourceRequest);
    if (!m_mainResource) {
        setRequest(ResourceRequest());
        m_applicationCacheHost = adoptPtr(new ApplicationCacheHost(this));
        maybeLoadEmpty();
        return;
    }

    if (!mainResourceLoader()) {
        m_identifierForLoadWithoutResourceLoader = m_frame->page()->progress()->createUniqueIdentifier();
        frameLoader()->notifier()->assignIdentifierToInitialRequest(m_identifierForLoadWithoutResourceLoader, this, request);
        frameLoader()->notifier()->dispatchWillSendRequest(this, m_identifierForLoadWithoutResourceLoader, request, ResourceResponse());
    }
    m_mainResource->addClient(this);

    if (mainResourceLoader())
        request = mainResourceLoader()->originalRequest();
    if (equalIgnoringFragmentIdentifier(m_request.url(), request.url()))
        request.setURL(m_request.url());
    setRequest(request);
}
