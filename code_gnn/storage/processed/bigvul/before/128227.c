void FrameView::contentRectangleForPaintInvalidation(const IntRect& r)
{
    ASSERT(paintInvalidationIsAllowed());
    ASSERT(!m_frame->owner());

    if (m_isTrackingPaintInvalidations) {
        IntRect paintInvalidationRect = r;
        paintInvalidationRect.move(-scrollOffset());
        m_trackedPaintInvalidationRects.append(paintInvalidationRect);
        RELEASE_ASSERT_NOT_REACHED();
    }

    ScrollView::contentRectangleForPaintInvalidation(r);
}
