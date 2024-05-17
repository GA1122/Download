void DocumentLoader::willSendRequest(ResourceRequest& newRequest, const ResourceResponse& redirectResponse)
{
    ASSERT(!newRequest.isNull());

    if (!frameLoader()->checkIfFormActionAllowedByCSP(newRequest.url())) {
        cancelMainResourceLoad(frameLoader()->cancelledError(newRequest));
        return;
    }

    ASSERT(timing()->fetchStart());
    if (!redirectResponse.isNull()) {
        RefPtr<SecurityOrigin> redirectingOrigin = SecurityOrigin::create(redirectResponse.url());
        if (!redirectingOrigin->canDisplay(newRequest.url())) {
            FrameLoader::reportLocalLoadFailed(m_frame, newRequest.url().string());
            cancelMainResourceLoad(frameLoader()->cancelledError(newRequest));
            return;
        }
        timing()->addRedirect(redirectResponse.url(), newRequest.url());
    }

    if (frameLoader()->isLoadingMainFrame())
        newRequest.setFirstPartyForCookies(newRequest.url());

    if (newRequest.cachePolicy() == UseProtocolCachePolicy && isPostOrRedirectAfterPost(newRequest, redirectResponse))
        newRequest.setCachePolicy(ReloadIgnoringCacheData);

    Frame* top = m_frame->tree()->top();
    if (top != m_frame) {
        if (!frameLoader()->mixedContentChecker()->canDisplayInsecureContent(top->document()->securityOrigin(), newRequest.url())) {
            cancelMainResourceLoad(frameLoader()->cancelledError(newRequest));
            return;
        }
    }

    setRequest(newRequest);

    if (!redirectResponse.isNull()) {
        ASSERT(!m_substituteData.isValid());
        m_applicationCacheHost->maybeLoadMainResourceForRedirect(newRequest, m_substituteData);
        if (m_substituteData.isValid())
            m_identifierForLoadWithoutResourceLoader = mainResourceLoader()->identifier();
    }

    if (!redirectResponse.isNull())
        frameLoader()->policyChecker()->checkNavigationPolicy(newRequest, callContinueAfterNavigationPolicy, this);
}
