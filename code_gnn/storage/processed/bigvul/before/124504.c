LayoutRect RenderBlock::blockSelectionGap(RenderBlock* rootBlock, const LayoutPoint& rootBlockPhysicalPosition, const LayoutSize& offsetFromRootBlock,
                                          LayoutUnit lastLogicalTop, LayoutUnit lastLogicalLeft, LayoutUnit lastLogicalRight, LayoutUnit logicalBottom, const PaintInfo* paintInfo)
{
    LayoutUnit logicalTop = lastLogicalTop;
    LayoutUnit logicalHeight = rootBlock->blockDirectionOffset(offsetFromRootBlock) + logicalBottom - logicalTop;
    if (logicalHeight <= 0)
        return LayoutRect();

    LayoutUnit logicalLeft = max(lastLogicalLeft, logicalLeftSelectionOffset(rootBlock, logicalBottom));
    LayoutUnit logicalRight = min(lastLogicalRight, logicalRightSelectionOffset(rootBlock, logicalBottom));
    LayoutUnit logicalWidth = logicalRight - logicalLeft;
    if (logicalWidth <= 0)
        return LayoutRect();

    LayoutRect gapRect = rootBlock->logicalRectToPhysicalRect(rootBlockPhysicalPosition, LayoutRect(logicalLeft, logicalTop, logicalWidth, logicalHeight));
    if (paintInfo)
        paintInfo->context->fillRect(pixelSnappedIntRect(gapRect), selectionBackgroundColor());
    return gapRect;
}
