void WebPageProxy::dismissCorrectionPanel(int32_t reason)
{
    m_pageClient->dismissCorrectionPanel((WebCore::ReasonForDismissingCorrectionPanel)reason);
}
