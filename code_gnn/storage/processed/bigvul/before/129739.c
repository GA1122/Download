ResourceRequestCachePolicy ResourceFetcher::resourceRequestCachePolicy(const ResourceRequest& request, Resource::Type type)
{
    if (type == Resource::MainResource) {
        FrameLoadType frameLoadType = frame()->loader().loadType();
        if (request.httpMethod() == "POST" && frameLoadType == FrameLoadTypeBackForward)
            return ReturnCacheDataDontLoad;
        if (!m_documentLoader->overrideEncoding().isEmpty() || frameLoadType == FrameLoadTypeBackForward)
            return ReturnCacheDataElseLoad;
        if (frameLoadType == FrameLoadTypeReloadFromOrigin)
            return ReloadBypassingCache;
        if (frameLoadType == FrameLoadTypeReload || frameLoadType == FrameLoadTypeSame || request.isConditional() || request.httpMethod() == "POST")
            return ReloadIgnoringCacheData;
        Frame* parent = frame()->tree().parent();
        if (parent && parent->isLocalFrame())
            return toLocalFrame(parent)->document()->fetcher()->resourceRequestCachePolicy(request, type);
        return UseProtocolCachePolicy;
    }

    if (request.isConditional())
        return ReloadIgnoringCacheData;

    if (m_documentLoader && m_document && !m_document->loadEventFinished()) {
        ResourceRequestCachePolicy mainResourceCachePolicy = m_documentLoader->request().cachePolicy();
        if (mainResourceCachePolicy == ReturnCacheDataDontLoad)
            return ReturnCacheDataElseLoad;
        return mainResourceCachePolicy;
    }
    return UseProtocolCachePolicy;
}
