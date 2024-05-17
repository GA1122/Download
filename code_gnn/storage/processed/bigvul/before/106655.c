bool WebPageProxy::supportsTextZoom() const
{
    if (m_mainFrameHasCustomRepresentation)
        return false;

    if (!m_mainFrame || m_mainFrame->isDisplayingStandaloneImageDocument())
        return false;

    return true;
}
