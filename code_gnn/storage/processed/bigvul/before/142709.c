void FrameLoader::setDefersLoading(bool defers)
{
    if (m_provisionalDocumentLoader)
        m_provisionalDocumentLoader->fetcher()->setDefersLoading(defers);

    if (Document* document = m_frame->document()) {
        document->fetcher()->setDefersLoading(defers);
        if (defers)
            document->suspendScheduledTasks();
        else
            document->resumeScheduledTasks();
    }

    if (!defers) {
        if (m_deferredHistoryLoad) {
            load(FrameLoadRequest(nullptr, m_deferredHistoryLoad->m_request),
                m_deferredHistoryLoad->m_loadType, m_deferredHistoryLoad->m_item.get(),
                m_deferredHistoryLoad->m_historyLoadType);
            m_deferredHistoryLoad.clear();
        }
        m_frame->navigationScheduler().startTimer();
        scheduleCheckCompleted();
    }
}
