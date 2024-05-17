void RenderBlockFlow::computeOverflow(LayoutUnit oldClientAfterEdge, bool recomputeFloats)
{
    RenderBlock::computeOverflow(oldClientAfterEdge, recomputeFloats);
    if (!hasColumns() && (recomputeFloats || createsBlockFormattingContext() || hasSelfPaintingLayer()))
        addOverflowFromFloats();
}
