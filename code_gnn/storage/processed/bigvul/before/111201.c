void WebPagePrivate::load(const BlackBerry::Platform::String& url, const BlackBerry::Platform::String& networkToken, const BlackBerry::Platform::String& method, Platform::NetworkRequest::CachePolicy cachePolicy, const char* data, size_t dataLength, const char* const* headers, size_t headersLength, bool isInitial, bool mustHandleInternally, bool forceDownload, const BlackBerry::Platform::String& overrideContentType, const BlackBerry::Platform::String& suggestedSaveName)
{
    stopCurrentLoad();
    DeferredTaskLoadManualScript::finishOrCancel(this);

    String urlString(url);
    if (urlString.startsWith("vs:", false)) {
        urlString = urlString.substring(3);
        m_mainFrame->setInViewSourceMode(true);
    } else
        m_mainFrame->setInViewSourceMode(false);

    KURL kurl = parseUrl(urlString);
    if (protocolIs(kurl, "javascript")) {
        if (m_page->defersLoading())
            m_deferredTasks.append(adoptPtr(new DeferredTaskLoadManualScript(this, kurl)));
        else
            m_mainFrame->script()->executeIfJavaScriptURL(kurl, DoNotReplaceDocumentIfJavaScriptURL);
        return;
    }

    if (isInitial)
        NetworkManager::instance()->setInitialURL(kurl);

    ResourceRequest request(kurl);
    request.setToken(networkToken);
    if (isInitial || mustHandleInternally)
        request.setMustHandleInternally(true);
    request.setHTTPMethod(method);
    request.setCachePolicy(toWebCoreCachePolicy(cachePolicy));
    if (!overrideContentType.empty())
        request.setOverrideContentType(overrideContentType);

    if (data)
        request.setHTTPBody(FormData::create(data, dataLength));

    for (unsigned i = 0; i + 1 < headersLength; i += 2)
        request.addHTTPHeaderField(headers[i], headers[i + 1]);

    if (forceDownload)
        request.setForceDownload(true);

    request.setSuggestedSaveName(suggestedSaveName);

    FrameLoadRequest frameRequest(m_mainFrame, request);
    frameRequest.setFrameName("");
    frameRequest.setShouldCheckNewWindowPolicy(true);
    m_mainFrame->loader()->load(frameRequest);
}
