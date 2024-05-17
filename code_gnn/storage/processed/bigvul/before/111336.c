void WebPagePrivate::setCompositorBackgroundColor(const Color& backgroundColor)
{
    if (m_compositor)
        m_compositor->setBackgroundColor(backgroundColor);
}
