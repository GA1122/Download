void WebPageProxy::didCancelForOpenPanel()
{
    if (!isValid())
        return;

    process()->send(Messages::WebPage::DidCancelForOpenPanel(), m_pageID);
    
    m_openPanelResultListener->invalidate();
    m_openPanelResultListener = 0;
}
