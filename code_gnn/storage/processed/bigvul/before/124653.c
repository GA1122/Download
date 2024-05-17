void RenderBlock::updateBlockChildDirtyBitsBeforeLayout(bool relayoutChildren, RenderBox* child)
{
    if (relayoutChildren || (child->hasRelativeLogicalHeight() && !isRenderView()))
        child->setChildNeedsLayout(MarkOnlyThis);

    if (relayoutChildren && child->needsPreferredWidthsRecalculation())
        child->setPreferredLogicalWidthsDirty(MarkOnlyThis);
}
