void WebPageProxy::setDrawsBackground(bool drawsBackground)
{
    if (m_drawsBackground == drawsBackground)
        return;

    m_drawsBackground = drawsBackground;

    if (isValid())
        process()->send(Messages::WebPage::SetDrawsBackground(drawsBackground), m_pageID);
}
