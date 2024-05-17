void FrameView::recalcOverflowAfterStyleChange()
{
    RenderView* renderView = this->renderView();
    ASSERT(renderView);
    if (!renderView->needsOverflowRecalcAfterStyleChange())
        return;

    renderView->recalcOverflowAfterStyleChange();

    IntRect documentRect = renderView->documentRect();
    if (scrollOrigin() == -documentRect.location() && contentsSize() == documentRect.size())
        return;

    if (needsLayout())
        return;

    InUpdateScrollbarsScope inUpdateScrollbarsScope(this);

    bool shouldHaveHorizontalScrollbar = false;
    bool shouldHaveVerticalScrollbar = false;
    computeScrollbarExistence(shouldHaveHorizontalScrollbar, shouldHaveVerticalScrollbar, documentRect.size());

    bool hasHorizontalScrollbar = horizontalScrollbar();
    bool hasVerticalScrollbar = verticalScrollbar();
    if (hasHorizontalScrollbar != shouldHaveHorizontalScrollbar
        || hasVerticalScrollbar != shouldHaveVerticalScrollbar) {
        setNeedsLayout();
        return;
    }

    adjustViewSize();
    updateScrollbarGeometry();
}
