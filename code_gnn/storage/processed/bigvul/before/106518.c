void WebPageProxy::endPrinting()
{
    if (!m_isInPrintingMode)
        return;

    m_isInPrintingMode = false;
    process()->send(Messages::WebPage::EndPrinting(), m_pageID, m_isPerformingDOMPrintOperation ? CoreIPC::DispatchMessageEvenWhenWaitingForSyncReply : 0);
}
