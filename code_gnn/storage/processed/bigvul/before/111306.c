void WebPagePrivate::resetBlockZoom()
{
    m_currentBlockZoomNode = 0;
    m_currentBlockZoomAdjustedNode = 0;
    m_shouldReflowBlock = false;
    m_shouldConstrainScrollingToContentEdge = true;
}
