void FrameView::invalidateTreeIfNeeded()
{
    RenderObject* rootForPaintInvalidation = renderView();
    ASSERT(!rootForPaintInvalidation->needsLayout());

    TRACE_EVENT1("blink", "FrameView::invalidateTree", "root", rootForPaintInvalidation->debugName().ascii());

    PaintInvalidationState rootPaintInvalidationState(*rootForPaintInvalidation);

    rootForPaintInvalidation->invalidateTreeIfNeeded(rootPaintInvalidationState);

    if (hasVerticalBarDamage())
        invalidateRect(verticalBarDamage());
    if (hasHorizontalBarDamage())
        invalidateRect(horizontalBarDamage());
    resetScrollbarDamage();

    m_doFullPaintInvalidation = false;
#ifndef NDEBUG
    renderView()->assertSubtreeClearedPaintInvalidationState();
#endif

    if (m_frame->selection().isCaretBoundsDirty())
        m_frame->selection().invalidateCaretRect();
}
