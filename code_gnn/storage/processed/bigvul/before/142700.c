void FrameLoader::receivedFirstData()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;

    HistoryCommitType historyCommitType = loadTypeToCommitType(m_loadType);
    if (historyCommitType == StandardCommit && (m_documentLoader->urlForHistory().isEmpty() || (opener() && !m_currentItem && m_documentLoader->originalRequest().url().isEmpty())))
        historyCommitType = HistoryInertCommit;
    else if (historyCommitType == InitialCommitInChildFrame && MixedContentChecker::isMixedContent(m_frame->tree().top()->securityContext()->getSecurityOrigin(), m_documentLoader->url()))
        historyCommitType = HistoryInertCommit;
    setHistoryItemStateForCommit(historyCommitType, HistoryNavigationType::DifferentDocument);

    if (!m_stateMachine.committedMultipleRealLoads() && m_loadType == FrameLoadTypeStandard)
        m_stateMachine.advanceTo(FrameLoaderStateMachine::CommittedMultipleRealLoads);

    client()->dispatchDidCommitLoad(m_currentItem.get(), historyCommitType);

    m_frame->securityContext()->contentSecurityPolicy()->reportAccumulatedHeaders(client());

    if (client()->isControlledByServiceWorker(*m_documentLoader))
        client()->didObserveLoadingBehavior(WebLoadingBehaviorServiceWorkerControlled);

    TRACE_EVENT1("devtools.timeline", "CommitLoad", "data", InspectorCommitLoadEvent::data(m_frame));
    InspectorInstrumentation::didCommitLoad(m_frame, m_documentLoader.get());
    m_frame->page()->didCommitLoad(m_frame);
    dispatchDidClearDocumentOfWindowObject();

    takeObjectSnapshot();
}
