void LayoutBlockFlow::layoutBlock(bool relayoutChildren)
{
    ASSERT(needsLayout());
    ASSERT(isInlineBlockOrInlineTable() || !isInline());

    m_hasOnlySelfCollapsingChildren = false;

    if (!relayoutChildren && simplifiedLayout())
        return;

    LayoutAnalyzer::BlockScope analyzer(*this);
    SubtreeLayoutScope layoutScope(*this);

    bool done = false;
    LayoutUnit pageLogicalHeight = 0;
    while (!done)
        done = layoutBlockFlow(relayoutChildren, pageLogicalHeight, layoutScope);

    LayoutView* layoutView = view();
    if (layoutView->layoutState()->pageLogicalHeight())
        setPageLogicalOffset(layoutView->layoutState()->pageLogicalOffset(*this, logicalTop()));

    updateLayerTransformAfterLayout();

    updateScrollInfoAfterLayout();

    if (m_paintInvalidationLogicalTop != m_paintInvalidationLogicalBottom) {
        bool hasVisibleContent = style()->visibility() == VISIBLE;
        if (!hasVisibleContent) {
            PaintLayer* layer = enclosingLayer();
            layer->updateDescendantDependentFlags();
            hasVisibleContent = layer->hasVisibleContent();
        }
        if (hasVisibleContent)
            setShouldInvalidateOverflowForPaint();
    }

    if (isHTMLDialogElement(node()) && isOutOfFlowPositioned())
        positionDialog();

    clearNeedsLayout();
}
