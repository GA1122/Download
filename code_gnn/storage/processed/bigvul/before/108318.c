void MainResourceLoader::willSendRequest(ResourceRequest& newRequest, const ResourceResponse& redirectResponse)
{
    ASSERT(!newRequest.isNull());
    
    RefPtr<MainResourceLoader> protect(this);

    ASSERT(documentLoader()->timing()->fetchStart);
    if (!redirectResponse.isNull()) {
        DocumentLoadTiming* documentLoadTiming = documentLoader()->timing();
        documentLoadTiming->redirectCount++;
        if (!documentLoadTiming->redirectStart)
            documentLoadTiming->redirectStart = documentLoadTiming->fetchStart;
        documentLoadTiming->redirectEnd = currentTime();
        documentLoadTiming->fetchStart = documentLoadTiming->redirectEnd;
    }

    if (frameLoader()->isLoadingMainFrame())
        newRequest.setFirstPartyForCookies(newRequest.url());
    
    if (newRequest.cachePolicy() == UseProtocolCachePolicy && isPostOrRedirectAfterPost(newRequest, redirectResponse))
        newRequest.setCachePolicy(ReloadIgnoringCacheData);

    ResourceLoader::willSendRequest(newRequest, redirectResponse);
    
    m_documentLoader->setRequest(newRequest);

    Frame* top = m_frame->tree()->top();
    if (top != m_frame)
        frameLoader()->checkIfDisplayInsecureContent(top->document()->securityOrigin(), newRequest.url());

    if (!redirectResponse.isNull()) {
        ref();  
        frameLoader()->policyChecker()->checkNavigationPolicy(newRequest, callContinueAfterNavigationPolicy, this);
    }
}
