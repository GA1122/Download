GapRects RenderBlock::selectionGaps(RenderBlock* rootBlock, const LayoutPoint& rootBlockPhysicalPosition, const LayoutSize& offsetFromRootBlock,
                                    LayoutUnit& lastLogicalTop, LayoutUnit& lastLogicalLeft, LayoutUnit& lastLogicalRight, const PaintInfo* paintInfo)
{
    if (paintInfo) {
        LayoutRect flippedBlockRect(offsetFromRootBlock.width(), offsetFromRootBlock.height(), width(), height());
        rootBlock->flipForWritingMode(flippedBlockRect);
        flippedBlockRect.moveBy(rootBlockPhysicalPosition);
        clipOutPositionedObjects(paintInfo, flippedBlockRect.location(), positionedObjects());
        if (isBody() || isDocumentElement())  
            for (RenderBlock* cb = containingBlock(); cb && !cb->isRenderView(); cb = cb->containingBlock())
                clipOutPositionedObjects(paintInfo, LayoutPoint(cb->x(), cb->y()), cb->positionedObjects());  
        clipOutFloatingObjects(rootBlock, paintInfo, rootBlockPhysicalPosition, offsetFromRootBlock);
    }

    GapRects result;
    if (!isRenderBlockFlow() && !isFlexibleBoxIncludingDeprecated())  
        return result;

    if (hasColumns() || hasTransform() || style()->columnSpan()) {
        lastLogicalTop = rootBlock->blockDirectionOffset(offsetFromRootBlock) + logicalHeight();
        lastLogicalLeft = logicalLeftSelectionOffset(rootBlock, logicalHeight());
        lastLogicalRight = logicalRightSelectionOffset(rootBlock, logicalHeight());
        return result;
    }

    if (childrenInline())
        result = toRenderBlockFlow(this)->inlineSelectionGaps(rootBlock, rootBlockPhysicalPosition, offsetFromRootBlock, lastLogicalTop, lastLogicalLeft, lastLogicalRight, paintInfo);
    else
        result = blockSelectionGaps(rootBlock, rootBlockPhysicalPosition, offsetFromRootBlock, lastLogicalTop, lastLogicalLeft, lastLogicalRight, paintInfo);

    if (rootBlock == this && (selectionState() != SelectionBoth && selectionState() != SelectionEnd))
        result.uniteCenter(blockSelectionGap(rootBlock, rootBlockPhysicalPosition, offsetFromRootBlock, lastLogicalTop, lastLogicalLeft, lastLogicalRight,
                                             logicalHeight(), paintInfo));
    return result;
}
