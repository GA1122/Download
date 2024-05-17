void FrameLoader::loadFailed(DocumentLoader* loader, const ResourceError& error)
{
    if (!error.isCancellation() && m_frame->owner()) {
        if (m_frame->owner()->isLocal())
            m_frame->deprecatedLocalOwner()->renderFallbackContent();
    }

    HistoryCommitType historyCommitType = loadTypeToCommitType(m_loadType);
    if (loader == m_provisionalDocumentLoader) {
        client()->dispatchDidFailProvisionalLoad(error, historyCommitType);
        if (loader != m_provisionalDocumentLoader)
            return;
        detachDocumentLoader(m_provisionalDocumentLoader);
        m_progressTracker->progressCompleted();
    } else {
        ASSERT(loader == m_documentLoader);
        if (m_frame->document()->parser())
            m_frame->document()->parser()->stopParsing();
        m_documentLoader->setSentDidFinishLoad();
        if (!m_provisionalDocumentLoader && m_frame->isLoading()) {
            client()->dispatchDidFailLoad(error, historyCommitType);
            m_progressTracker->progressCompleted();
        }
    }
    checkCompleted();
}
