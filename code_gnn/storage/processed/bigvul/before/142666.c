void FrameLoader::checkCompleted()
{
    if (!shouldComplete(m_frame->document()))
        return;

    m_frame->document()->setReadyState(Document::Complete);
    if (m_frame->document()->loadEventStillNeeded())
        m_frame->document()->implicitClose();

    m_frame->navigationScheduler().startTimer();

    if (m_frame->view())
        m_frame->view()->handleLoadCompleted();

    if (!m_frame->client())
        return;

    if (shouldSendFinishNotification(m_frame)) {
        if (m_frame->isMainFrame())
            m_frame->document()->viewportDescription().reportMobilePageStats(m_frame);
        m_documentLoader->setSentDidFinishLoad();
        client()->dispatchDidFinishLoad();
        if (!m_frame->client())
            return;
    }

    if (shouldSendCompleteNotification(m_frame)) {
        m_progressTracker->progressCompleted();
        restoreScrollPositionAndViewState();

        m_loadType = FrameLoadTypeStandard;
        m_frame->localDOMWindow()->finishedLoading();
    }

    Frame* parent = m_frame->tree().parent();
    if (parent && parent->isLocalFrame())
        toLocalFrame(parent)->loader().checkCompleted();
}
