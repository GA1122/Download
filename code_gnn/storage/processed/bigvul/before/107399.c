void Scrollbar::setFrameRect(const IntRect& rect)
{
    IntRect adjustedRect(rect);
    bool overlapsResizer = false;
    ScrollView* view = parent();
    if (view && !rect.isEmpty() && !view->windowResizerRect().isEmpty()) {
        IntRect resizerRect = view->convertFromContainingWindow(view->windowResizerRect());
        if (rect.intersects(resizerRect)) {
            if (orientation() == HorizontalScrollbar) {
                int overlap = rect.maxX() - resizerRect.x();
                if (overlap > 0 && resizerRect.maxX() >= rect.maxX()) {
                    adjustedRect.setWidth(rect.width() - overlap);
                    overlapsResizer = true;
                }
            } else {
                int overlap = rect.maxY() - resizerRect.y();
                if (overlap > 0 && resizerRect.maxY() >= rect.maxY()) {
                    adjustedRect.setHeight(rect.height() - overlap);
                    overlapsResizer = true;
                }
            }
        }
    }
    if (overlapsResizer != m_overlapsResizer) {
        m_overlapsResizer = overlapsResizer;
        if (view)
            view->adjustScrollbarsAvoidingResizerCount(m_overlapsResizer ? 1 : -1);
    }

    Widget::setFrameRect(adjustedRect);
}
