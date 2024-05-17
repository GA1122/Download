void WebPageProxy::dismissCorrectionPanelSoon(int32_t reason, String& result)
{
    result = m_pageClient->dismissCorrectionPanelSoon((WebCore::ReasonForDismissingCorrectionPanel)reason);
}
