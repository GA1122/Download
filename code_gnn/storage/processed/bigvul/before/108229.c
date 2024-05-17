void FrameLoader::addExtraFieldsToRequest(ResourceRequest& request, FrameLoadType loadType, bool mainResource, bool cookiePolicyURLFromRequest)
{
    if (request.firstPartyForCookies().isEmpty()) {
        if (mainResource && (isLoadingMainFrame() || cookiePolicyURLFromRequest))
            request.setFirstPartyForCookies(request.url());
        else if (Document* document = m_frame->document())
            request.setFirstPartyForCookies(document->firstPartyForCookies());
    }

    if (!request.url().isEmpty() && !request.url().protocolInHTTPFamily())
        return;

    applyUserAgent(request);
    
    if (!mainResource) {
        if (request.isConditional())
            request.setCachePolicy(ReloadIgnoringCacheData);
        else if (documentLoader()->isLoadingInAPISense())
            request.setCachePolicy(documentLoader()->originalRequest().cachePolicy());
        else
            request.setCachePolicy(UseProtocolCachePolicy);
    } else if (loadType == FrameLoadTypeReload) {
        request.setCachePolicy(ReloadIgnoringCacheData);
        request.setHTTPHeaderField("Cache-Control", "max-age=0");
    } else if (loadType == FrameLoadTypeReloadFromOrigin) {
        request.setCachePolicy(ReloadIgnoringCacheData);
        request.setHTTPHeaderField("Cache-Control", "no-cache");
        request.setHTTPHeaderField("Pragma", "no-cache");
    } else if (request.isConditional())
        request.setCachePolicy(ReloadIgnoringCacheData);
    else if (isBackForwardLoadType(loadType) && m_stateMachine.committedFirstRealDocumentLoad() && !request.url().protocolIs("https"))
        request.setCachePolicy(ReturnCacheDataElseLoad);
    
    if (mainResource)
        request.setHTTPAccept(defaultAcceptHeader);

    addHTTPOriginIfNeeded(request, String());

    Settings* settings = m_frame->settings();
    request.setResponseContentDispositionEncodingFallbackArray("UTF-8", writer()->deprecatedFrameEncoding(), settings ? settings->defaultTextEncodingName() : String());
}
