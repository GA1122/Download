void FrameView::viewportConstrainedVisibleContentSizeChanged(bool widthChanged, bool heightChanged)
{
    if (!hasViewportConstrainedObjects())
        return;

    if (m_frame->settings()
        && !m_frame->settings()->viewportEnabled()
        && !m_frame->settings()->pinchVirtualViewportEnabled())
        return;

    ViewportConstrainedObjectSet::const_iterator end = m_viewportConstrainedObjects->end();
    for (ViewportConstrainedObjectSet::const_iterator it = m_viewportConstrainedObjects->begin(); it != end; ++it) {
        RenderObject* renderer = *it;
        RenderStyle* style = renderer->style();
        if (widthChanged) {
            if (style->width().isFixed() && (style->left().isAuto() || style->right().isAuto()))
                renderer->setNeedsPositionedMovementLayout();
            else
                renderer->setNeedsLayoutAndFullPaintInvalidation();
        }
        if (heightChanged) {
            if (style->height().isFixed() && (style->top().isAuto() || style->bottom().isAuto()))
                renderer->setNeedsPositionedMovementLayout();
            else
                renderer->setNeedsLayoutAndFullPaintInvalidation();
        }
    }
}
