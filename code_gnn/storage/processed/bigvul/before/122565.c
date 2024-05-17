void WebDevToolsAgentImpl::highlight()
{
    m_webViewImpl->addPageOverlay(this, OverlayZOrders::highlight);
}
