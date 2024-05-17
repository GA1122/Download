void LayoutBlockFlow::updateBlockChildDirtyBitsBeforeLayout(bool relayoutChildren, LayoutBox& child)
{
    if (child.isLayoutMultiColumnSpannerPlaceholder())
        toLayoutMultiColumnSpannerPlaceholder(child).markForLayoutIfObjectInFlowThreadNeedsLayout();
    LayoutBlock::updateBlockChildDirtyBitsBeforeLayout(relayoutChildren, child);
}
