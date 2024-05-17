void FrameView::paintScrollCorner(GraphicsContext* context, const IntRect& cornerRect)
{
    if (m_scrollCorner) {
        bool needsBackgorund = m_frame->isMainFrame();
        if (needsBackgorund)
            context->fillRect(cornerRect, baseBackgroundColor());
        m_scrollCorner->paintIntoRect(context, cornerRect.location(), cornerRect);
        return;
    }

    ScrollView::paintScrollCorner(context, cornerRect);
}
