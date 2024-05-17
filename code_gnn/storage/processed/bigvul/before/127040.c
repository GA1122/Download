void ChromeClientImpl::numWheelEventHandlersChanged(unsigned numberOfWheelHandlers)
{
    m_webView->numberOfWheelEventHandlersChanged(numberOfWheelHandlers);
}
