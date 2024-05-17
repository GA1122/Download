void WebFrame::invalidatePolicyListener()
{
    if (!m_policyListenerID)
        return;

    m_policyDownloadID = 0;
    m_policyListenerID = 0;
    m_policyFunction = 0;
}
