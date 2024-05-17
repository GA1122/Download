void WebPageProxy::getIsResizable(bool& isResizable)
{
    isResizable = m_uiClient.isResizable(this);
}
