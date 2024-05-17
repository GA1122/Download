void WebFrame::didReceivePolicyDecision(uint64_t listenerID, PolicyAction action, uint64_t downloadID)
{
    if (!m_coreFrame)
        return;

    if (!m_policyListenerID)
        return;

    if (listenerID != m_policyListenerID)
        return;

    ASSERT(m_policyFunction);

    FramePolicyFunction function = m_policyFunction;

    invalidatePolicyListener();

    m_policyDownloadID = downloadID;

    (m_coreFrame->loader()->policyChecker()->*function)(action);
}
