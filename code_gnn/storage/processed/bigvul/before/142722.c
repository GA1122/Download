void FrameLoader::stopAllLoaders()
{
    if (m_frame->document()->pageDismissalEventBeingDispatched() != Document::NoDismissal)
        return;

    if (m_inStopAllLoaders)
        return;

    m_inStopAllLoaders = true;

    for (Frame* child = m_frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (child->isLocalFrame())
            toLocalFrame(child)->loader().stopAllLoaders();
    }

    m_frame->document()->suppressLoadEvent();
    if (m_documentLoader)
        m_documentLoader->fetcher()->stopFetching();
    m_frame->document()->cancelParsing();
    if (!m_protectProvisionalLoader)
        detachDocumentLoader(m_provisionalDocumentLoader);

    m_checkTimer.stop();
    m_frame->navigationScheduler().cancel();

    if (!m_provisionalDocumentLoader && m_documentLoader && m_frame->isLoading())
        loadFailed(m_documentLoader.get(), ResourceError::cancelledError(m_documentLoader->url()));

    m_inStopAllLoaders = false;
    takeObjectSnapshot();
}
