void WebPageProxy::handleCorrectionPanelResult(const String& result)
{
#if !defined(BUILDING_ON_SNOW_LEOPARD)
    if (!isClosed())
        process()->send(Messages::WebPage::HandleCorrectionPanelResult(result), m_pageID, 0);
#endif
}
