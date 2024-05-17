void WebPagePrivate::contentsSizeChanged(const IntSize& contentsSize)
{
    if (m_previousContentsSize == contentsSize)
        return;

    m_contentsSizeChanged = true;

#if DEBUG_WEBPAGE_LOAD
    BBLOG(Platform::LogLevelInfo, "WebPagePrivate::contentsSizeChanged %dx%d", contentsSize.width(), contentsSize.height());
#endif
}
