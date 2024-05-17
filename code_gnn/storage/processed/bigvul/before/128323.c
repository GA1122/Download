void FrameView::scrollContentsSlowPath(const IntRect& updateRect)
{
    if (contentsInCompositedLayer()) {
        IntRect updateRect = visibleContentRect();
        ASSERT(renderView());
        renderView()->layer()->paintInvalidator().setBackingNeedsPaintInvalidationInRect(updateRect);
    }
    if (RenderPart* frameRenderer = m_frame->ownerRenderer()) {
        if (isEnclosedInCompositingLayer()) {
            LayoutRect rect(frameRenderer->borderLeft() + frameRenderer->paddingLeft(),
                            frameRenderer->borderTop() + frameRenderer->paddingTop(),
                            visibleWidth(), visibleHeight());
            frameRenderer->invalidatePaintRectangle(rect);
            return;
        }
    }

    ScrollView::scrollContentsSlowPath(updateRect);
}
