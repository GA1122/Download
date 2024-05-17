void WebPageProxy::setStatusText(const String& text)
{
    m_uiClient.setStatusText(this, text);
}
