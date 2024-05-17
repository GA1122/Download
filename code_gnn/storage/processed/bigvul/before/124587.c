LayoutRect RenderBlock::logicalRightSelectionGap(RenderBlock* rootBlock, const LayoutPoint& rootBlockPhysicalPosition, const LayoutSize& offsetFromRootBlock,
                                                 RenderObject* selObj, LayoutUnit logicalRight, LayoutUnit logicalTop, LayoutUnit logicalHeight, const PaintInfo* paintInfo)
{
    LayoutUnit rootBlockLogicalTop = rootBlock->blockDirectionOffset(offsetFromRootBlock) + logicalTop;
    LayoutUnit rootBlockLogicalLeft = max(rootBlock->inlineDirectionOffset(offsetFromRootBlock) + floorToInt(logicalRight), max(logicalLeftSelectionOffset(rootBlock, logicalTop), logicalLeftSelectionOffset(rootBlock, logicalTop + logicalHeight)));
    LayoutUnit rootBlockLogicalRight = min(logicalRightSelectionOffset(rootBlock, logicalTop), logicalRightSelectionOffset(rootBlock, logicalTop + logicalHeight));
    LayoutUnit rootBlockLogicalWidth = rootBlockLogicalRight - rootBlockLogicalLeft;
    if (rootBlockLogicalWidth <= 0)
        return LayoutRect();

    LayoutRect gapRect = rootBlock->logicalRectToPhysicalRect(rootBlockPhysicalPosition, LayoutRect(rootBlockLogicalLeft, rootBlockLogicalTop, rootBlockLogicalWidth, logicalHeight));
    if (paintInfo)
        paintInfo->context->fillRect(pixelSnappedIntRect(gapRect), selObj->selectionBackgroundColor());
    return gapRect;
}
