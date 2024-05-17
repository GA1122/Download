IntPoint WebPagePrivate::calculateReflowedScrollPosition(const FloatPoint& anchorOffset, double inverseScale)
{
    ASSERT(m_webPage->settings()->textReflowMode() == WebSettings::TextReflowEnabled);

    int offsetY = 0;
    int offsetX = 0;

    IntRect nodeRect = rectForNode(m_currentPinchZoomNode.get());

    if (m_currentPinchZoomNode->renderer() && m_anchorInNodeRectRatio.y() >= 0) {
        offsetY = nodeRect.height() * m_anchorInNodeRectRatio.y();
        if (m_currentPinchZoomNode->renderer()->isImage() && m_anchorInNodeRectRatio.x() > 0)
            offsetX = nodeRect.width() * m_anchorInNodeRectRatio.x() - anchorOffset.x() / inverseScale;
    }

    IntRect reflowedRect = adjustRectOffsetForFrameOffset(nodeRect, m_currentPinchZoomNode.get());

    return IntPoint(max(0, static_cast<int>(roundf(reflowedRect.x() + offsetX))),
                    max(0, static_cast<int>(roundf(reflowedRect.y() + offsetY - anchorOffset.y() / inverseScale))));
}
