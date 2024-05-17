void WebPageProxy::drawHeader(WebFrameProxy* frame, const FloatRect& rect)
{
    m_uiClient.drawHeader(this, frame, rect);
}
