bool WebPagePrivate::respectViewport() const
{
    return m_forceRespectViewportArguments || contentsSize().width() <= m_virtualViewportSize.width() + 1;
}
