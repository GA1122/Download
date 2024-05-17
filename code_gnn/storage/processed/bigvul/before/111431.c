void WebPagePrivate::zoomBlock()
{
    if (!m_mainFrame)
        return;

    IntPoint anchor(roundUntransformedPoint(m_finalBlockPoint));
    bool willUseTextReflow = false;

#if ENABLE(VIEWPORT_REFLOW)
    willUseTextReflow = m_webPage->settings()->textReflowMode() != WebSettings::TextReflowDisabled;
    toggleTextReflowIfEnabledForBlockZoomOnly(m_shouldReflowBlock);
    setNeedsLayout();
#endif

    TransformationMatrix zoom;
    zoom.scale(m_blockZoomFinalScale);
    *m_transformationMatrix = zoom;
    m_backingStore->d->suspendBackingStoreUpdates();
    m_backingStore->d->suspendScreenUpdates();
    updateViewportSize();

    FrameView* mainFrameView = m_mainFrame->view();
    bool constrainsScrollingToContentEdge = true;
    if (mainFrameView) {
        constrainsScrollingToContentEdge = mainFrameView->constrainsScrollingToContentEdge();
        mainFrameView->setConstrainsScrollingToContentEdge(m_shouldConstrainScrollingToContentEdge);
    }

#if ENABLE(VIEWPORT_REFLOW)
    requestLayoutIfNeeded();
    if (willUseTextReflow && m_shouldReflowBlock) {
        IntRect reflowedRect = rectForNode(m_currentBlockZoomAdjustedNode.get());
        reflowedRect = adjustRectOffsetForFrameOffset(reflowedRect, m_currentBlockZoomAdjustedNode.get());
        reflowedRect.move(roundTransformedPoint(m_finalBlockPointReflowOffset).x(), roundTransformedPoint(m_finalBlockPointReflowOffset).y());
        RenderObject* renderer = m_currentBlockZoomAdjustedNode->renderer();
        IntPoint topLeftPoint(reflowedRect.location());
        if (renderer && renderer->isText()) {
            ETextAlign textAlign = renderer->style()->textAlign();
            IntPoint textAnchor;
            switch (textAlign) {
            case CENTER:
            case WEBKIT_CENTER:
                textAnchor = IntPoint(reflowedRect.x() + (reflowedRect.width() - actualVisibleSize().width()) / 2, topLeftPoint.y());
                break;
            case LEFT:
            case WEBKIT_LEFT:
                textAnchor = topLeftPoint;
                break;
            case RIGHT:
            case WEBKIT_RIGHT:
                textAnchor = IntPoint(reflowedRect.x() + reflowedRect.width() - actualVisibleSize().width(), topLeftPoint.y());
                break;
            case TAAUTO:
            case JUSTIFY:
            default:
                if (renderer->style()->isLeftToRightDirection())
                    textAnchor = topLeftPoint;
                else
                    textAnchor = IntPoint(reflowedRect.x() + reflowedRect.width() - actualVisibleSize().width(), topLeftPoint.y());
                break;
            }
            setScrollPosition(textAnchor);
        } else {
            renderer->style()->isLeftToRightDirection()
                ? setScrollPosition(topLeftPoint)
                : setScrollPosition(IntPoint(reflowedRect.x() + reflowedRect.width() - actualVisibleSize().width(), topLeftPoint.y()));
        }
    } else if (willUseTextReflow) {
        IntRect finalRect = rectForNode(m_currentBlockZoomAdjustedNode.get());
        finalRect = adjustRectOffsetForFrameOffset(finalRect, m_currentBlockZoomAdjustedNode.get());
        setScrollPosition(IntPoint(0, finalRect.y() + m_finalBlockPointReflowOffset.y()));
        resetBlockZoom();
    }
#endif
    if (!willUseTextReflow) {
        setScrollPosition(anchor);
        if (!m_shouldReflowBlock)
            resetBlockZoom();
    }

    notifyTransformChanged();
    m_client->scaleChanged();

    if (mainFrameView)
        mainFrameView->setConstrainsScrollingToContentEdge(constrainsScrollingToContentEdge);

    m_backingStore->d->resumeBackingStoreUpdates();
    m_backingStore->d->resumeScreenUpdates(BackingStore::RenderAndBlit);
}
