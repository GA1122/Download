void WebPageProxy::preferencesDidChange()
{
    if (!isValid())
        return;


    process()->send(Messages::WebPage::PreferencesDidChange(pageGroup()->preferences()->store()), m_pageID, m_isPerformingDOMPrintOperation ? CoreIPC::DispatchMessageEvenWhenWaitingForSyncReply : 0);
}
