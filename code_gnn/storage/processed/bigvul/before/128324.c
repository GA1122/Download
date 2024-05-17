void FrameView::scrollElementToRect(Element* element, const IntRect& rect)
{
    IntRect targetRect(rect);

    m_frame->document()->updateLayoutIgnorePendingStylesheets();

    bool pinchVirtualViewportEnabled = m_frame->settings()->pinchVirtualViewportEnabled();

    if (pinchVirtualViewportEnabled) {
        PinchViewport& pinchViewport = m_frame->page()->frameHost().pinchViewport();

        IntSize pinchViewportSize = expandedIntSize(pinchViewport.visibleRect().size());
        targetRect.moveBy(ceiledIntPoint(pinchViewport.visibleRect().location()));
        targetRect.setSize(pinchViewportSize.shrunkTo(targetRect.size()));
    }

    LayoutRect bounds = element->boundingBox();
    int centeringOffsetX = (targetRect.width() - bounds.width()) / 2;
    int centeringOffsetY = (targetRect.height() - bounds.height()) / 2;

    IntPoint targetOffset(
        bounds.x() - centeringOffsetX - targetRect.x(),
        bounds.y() - centeringOffsetY - targetRect.y());

    setScrollPosition(targetOffset);

    if (pinchVirtualViewportEnabled) {
        IntPoint remainder = IntPoint(targetOffset - scrollPosition());
        m_frame->page()->frameHost().pinchViewport().move(remainder);
    }
}
