void FrameLoader::commitProvisionalLoad()
{
    RefPtr<CachedPage> cachedPage = m_loadingFromCachedPage ? pageCache()->get(history()->provisionalItem()) : 0;
    RefPtr<DocumentLoader> pdl = m_provisionalDocumentLoader;

    LOG(PageCache, "WebCoreLoading %s: About to commit provisional load from previous URL '%s' to new URL '%s'", m_frame->tree()->name().string().utf8().data(), m_URL.string().utf8().data(), 
        pdl ? pdl->url().string().utf8().data() : "<no provisional DocumentLoader>");

    HistoryItem* item = history()->currentItem();
    if (!m_frame->tree()->parent() && PageCache::canCache(m_frame->page()) && !item->isInPageCache())
        pageCache()->add(item, m_frame->page());
    
    if (m_loadType != FrameLoadTypeReplace)
        closeOldDataSources();
    
    if (!cachedPage && !m_stateMachine.creatingInitialEmptyDocument())
        m_client->makeRepresentation(pdl.get());
    
    transitionToCommitted(cachedPage);
    
    if (m_sentRedirectNotification)
        clientRedirectCancelledOrFinished(false);
    
    if (cachedPage && cachedPage->document()) {
        prepareForCachedPageRestore();
        cachedPage->restore(m_frame->page());
        checkCompleted();
    } else {        
        KURL url = pdl->substituteData().responseURL();
        if (url.isEmpty())
            url = pdl->url();
        if (url.isEmpty())
            url = pdl->responseURL();
        if (url.isEmpty())
            url = blankURL();

        didOpenURL(url);
    }

    LOG(Loading, "WebCoreLoading %s: Finished committing provisional load to URL %s", m_frame->tree()->name().string().utf8().data(), m_URL.string().utf8().data());

    if (m_loadType == FrameLoadTypeStandard && m_documentLoader->isClientRedirect())
        history()->updateForClientRedirect();

    if (m_loadingFromCachedPage) {
        m_frame->document()->documentDidBecomeActive();
        
        m_frame->view()->forceLayout();

        const ResponseVector& responses = m_documentLoader->responses();
        size_t count = responses.size();
        for (size_t i = 0; i < count; i++) {
            const ResourceResponse& response = responses[i];
            ResourceError error;
            unsigned long identifier;
            ResourceRequest request(response.url());
            requestFromDelegate(request, identifier, error);
            notifier()->sendRemainingDelegateMessages(m_documentLoader.get(), identifier, response, static_cast<int>(response.expectedContentLength()), error);
        }
        
        pageCache()->remove(history()->currentItem());

        m_documentLoader->setPrimaryLoadComplete(true);

        checkLoadCompleteForThisFrame();
    }
}
