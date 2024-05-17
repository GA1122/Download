LayoutUnit RenderBlockFlow::marginOffsetForSelfCollapsingBlock()
{
    ASSERT(isSelfCollapsingBlock());
    RenderBlockFlow* parentBlock = toRenderBlockFlow(parent());
    if (parentBlock && style()->clear() && parentBlock->getClearDelta(this, logicalHeight()))
        return marginValuesForChild(this).positiveMarginBefore();
    return LayoutUnit();
}
