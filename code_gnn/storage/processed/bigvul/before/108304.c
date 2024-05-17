void MainResourceLoader::didReceiveResponse(const ResourceResponse& r)
{
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    if (documentLoader()->applicationCacheHost()->maybeLoadFallbackForMainResponse(request(), r))
        return;
#endif

    HTTPHeaderMap::const_iterator it = r.httpHeaderFields().find(AtomicString("x-frame-options"));
    if (it != r.httpHeaderFields().end()) {
        String content = it->second;
        if (m_frame->loader()->shouldInterruptLoadForXFrameOptions(content, r.url())) {
            cancel();
            return;
        }
    }

#if !PLATFORM(CF)
    ASSERT(shouldLoadAsEmptyDocument(r.url()) || !defersLoading());
#endif

#if PLATFORM(QT)
    if (r.mimeType() == "application/octet-stream")
        substituteMIMETypeFromPluginDatabase(r);
#endif

    if (m_loadingMultipartContent) {
        frameLoader()->setupForReplaceByMIMEType(r.mimeType());
        clearResourceData();
    }
    
    if (r.isMultipart())
        m_loadingMultipartContent = true;
        
    RefPtr<MainResourceLoader> protect(this);

    m_documentLoader->setResponse(r);

    m_response = r;

    ASSERT(!m_waitingForContentPolicy);
    m_waitingForContentPolicy = true;
    ref();  

    ASSERT(frameLoader()->activeDocumentLoader());

    if (frameLoader()->activeDocumentLoader()->substituteData().isValid()) {
        callContinueAfterContentPolicy(this, PolicyUse);
        return;
    }

#if ENABLE(FTPDIR)
    Settings* settings = m_frame->settings();
    if (settings && settings->forceFTPDirectoryListings() && m_response.mimeType() == "application/x-ftp-directory") {
        callContinueAfterContentPolicy(this, PolicyUse);
        return;
    }
#endif

    frameLoader()->policyChecker()->checkContentPolicy(m_response.mimeType(), callContinueAfterContentPolicy, this);
}
