void WebPageProxy::initializeUIClient(const WKPageUIClient* client)
{
    if (!isValid())
        return;

    m_uiClient.initialize(client);

    process()->send(Messages::WebPage::SetCanRunBeforeUnloadConfirmPanel(m_uiClient.canRunBeforeUnloadConfirmPanel()), m_pageID);
    process()->send(Messages::WebPage::SetCanRunModal(m_uiClient.canRunModal()), m_pageID);
}
