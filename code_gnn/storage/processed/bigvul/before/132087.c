LayoutRect LayoutBlockFlow::logicalLeftSelectionGap(const LayoutBlock* rootBlock, const LayoutPoint& rootBlockPhysicalPosition, const LayoutSize& offsetFromRootBlock,
    const LayoutObject* selObj, LayoutUnit logicalLeft, LayoutUnit logicalTop, LayoutUnit logicalHeight, const PaintInfo* paintInfo) const
{
    LayoutUnit rootBlockLogicalTop = rootBlock->blockDirectionOffset(offsetFromRootBlock) + logicalTop;
    LayoutUnit rootBlockLogicalLeft = std::max(logicalLeftSelectionOffset(rootBlock, logicalTop), logicalLeftSelectionOffset(rootBlock, logicalTop + logicalHeight));
    LayoutUnit rootBlockLogicalRight = std::min(rootBlock->inlineDirectionOffset(offsetFromRootBlock) + logicalLeft, std::min(logicalRightSelectionOffset(rootBlock, logicalTop), logicalRightSelectionOffset(rootBlock, logicalTop + logicalHeight)));
    LayoutUnit rootBlockLogicalWidth = rootBlockLogicalRight - rootBlockLogicalLeft;
    if (rootBlockLogicalWidth <= 0)
        return LayoutRect();

    LayoutRect gapRect = rootBlock->logicalRectToPhysicalRect(rootBlockPhysicalPosition, LayoutRect(rootBlockLogicalLeft, rootBlockLogicalTop, rootBlockLogicalWidth, logicalHeight));
    if (paintInfo) {
        IntRect selectionGapRect = alignSelectionRectToDevicePixels(gapRect);
        paintInfo->context->fillRect(selectionGapRect, selObj->selectionBackgroundColor());
    }
    return gapRect;
}
