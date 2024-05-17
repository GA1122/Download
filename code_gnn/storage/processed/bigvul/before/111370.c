void WebPagePrivate::setTextReflowAnchorPoint(const Platform::IntPoint& documentFocalPoint)
{
    ASSERT(m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabled);

    m_currentPinchZoomNode = bestNodeForZoomUnderPoint(documentFocalPoint);
    if (!m_currentPinchZoomNode)
        return;

    IntRect nodeRect = rectForNode(m_currentPinchZoomNode.get());
    m_anchorInNodeRectRatio.set(
        static_cast<float>(documentFocalPoint.x() - nodeRect.x()) / nodeRect.width(),
        static_cast<float>(documentFocalPoint.y() - nodeRect.y()) / nodeRect.height());
}
