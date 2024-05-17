void FrameLoader::detach()
{
    detachDocumentLoader(m_documentLoader);
    detachDocumentLoader(m_provisionalDocumentLoader);

    Frame* parent = m_frame->tree().parent();
    if (parent && parent->isLocalFrame())
        toLocalFrame(parent)->loader().scheduleCheckCompleted();
    if (m_progressTracker) {
        m_progressTracker->dispose();
        m_progressTracker.clear();
    }

    TRACE_EVENT_OBJECT_DELETED_WITH_ID("loading", "FrameLoader", this);
}
