FrameLoadType FrameLoader::determineFrameLoadType(const FrameLoadRequest& request)
{
    if (m_frame->tree().parent() && !m_stateMachine.committedFirstRealDocumentLoad())
        return FrameLoadTypeInitialInChildFrame;
    if (!m_frame->tree().parent() && !client()->backForwardLength())
        return FrameLoadTypeStandard;
    if (m_provisionalDocumentLoader && request.substituteData().failingURL() == m_provisionalDocumentLoader->url() && m_loadType == FrameLoadTypeBackForward)
        return FrameLoadTypeBackForward;
    if (request.resourceRequest().getCachePolicy() == WebCachePolicy::ValidatingCacheData)
        return FrameLoadTypeReload;
    if (request.resourceRequest().getCachePolicy() == WebCachePolicy::BypassingCache)
        return FrameLoadTypeReloadBypassingCache;
    if (request.replacesCurrentItem()
        || (!m_stateMachine.committedMultipleRealLoads()
            && equalIgnoringCase(m_frame->document()->url(), blankURL())))
        return FrameLoadTypeReplaceCurrentItem;

    if (request.resourceRequest().url() == m_documentLoader->urlForHistory()) {
        if (!request.originDocument())
            return FrameLoadTypeReloadMainResource;
        return request.resourceRequest().httpMethod() == HTTPNames::POST ? FrameLoadTypeStandard : FrameLoadTypeReplaceCurrentItem;
    }

    if (request.substituteData().failingURL() == m_documentLoader->urlForHistory() && m_loadType == FrameLoadTypeReload)
        return FrameLoadTypeReload;
    return FrameLoadTypeStandard;
}
