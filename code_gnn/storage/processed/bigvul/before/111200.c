void WebPagePrivate::layoutFinished()
{
    if (!m_contentsSizeChanged && !m_overflowExceedsContentsSize)
        return;

    m_contentsSizeChanged = false;  
    m_overflowExceedsContentsSize = false;

    if (contentsSize().isEmpty())
        return;

    if (didLayoutExceedMaximumIterations()) {
        notifyTransformedContentsSizeChanged();
        return;
    }

    IntSize previousContentsSize = m_previousContentsSize;

    m_nestedLayoutFinishedCount++;

    if (shouldZoomToInitialScaleOnLoad()) {
        zoomToInitialScaleOnLoad();
        m_shouldZoomToInitialScaleAfterLoadFinished = false;
    } else if (loadState() != None)
        notifyTransformedContentsSizeChanged();

    m_nestedLayoutFinishedCount--;

    if (!m_nestedLayoutFinishedCount) {

        if (contentsSize() != previousContentsSize) {

            IntPoint newScrollPosition = scrollPosition();

            if (contentsSize().height() < previousContentsSize.height()) {
                IntPoint scrollPositionWithHeightShrunk = IntPoint(newScrollPosition.x(), maximumScrollPosition().y());
                newScrollPosition = newScrollPosition.shrunkTo(scrollPositionWithHeightShrunk);
            }

            if (contentsSize().width() < previousContentsSize.width()) {
                IntPoint scrollPositionWithWidthShrunk = IntPoint(maximumScrollPosition().x(), newScrollPosition.y());
                newScrollPosition = newScrollPosition.shrunkTo(scrollPositionWithWidthShrunk);
            }

            if (newScrollPosition != scrollPosition()) {
                setScrollPosition(newScrollPosition);
                notifyTransformedScrollChanged();
            }
        }
    }
}
