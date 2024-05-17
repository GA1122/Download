void WebPageProxy::setMemoryCacheClientCallsEnabled(bool memoryCacheClientCallsEnabled)
{
    if (!isValid())
        return;

    if (m_areMemoryCacheClientCallsEnabled == memoryCacheClientCallsEnabled)
        return;

    m_areMemoryCacheClientCallsEnabled = memoryCacheClientCallsEnabled;
    process()->send(Messages::WebPage::SetMemoryCacheMessagesEnabled(memoryCacheClientCallsEnabled), m_pageID);
}
