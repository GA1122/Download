GapRects LayoutBlockFlow::selectionGaps(const LayoutBlock* rootBlock, const LayoutPoint& rootBlockPhysicalPosition,
    const LayoutSize& offsetFromRootBlock, LayoutUnit& lastLogicalTop, LayoutUnit& lastLogicalLeft, LayoutUnit& lastLogicalRight,
    const PaintInfo* paintInfo, ClipScope* clipScope) const
{
    if (clipScope) {
        LayoutRect flippedBlockRect(LayoutPoint(offsetFromRootBlock), size());
        rootBlock->flipForWritingMode(flippedBlockRect);
        flippedBlockRect.moveBy(rootBlockPhysicalPosition);
        clipOutPositionedObjects(*clipScope, flippedBlockRect.location(), positionedObjects());
        if (isBody() || isDocumentElement())  
            for (LayoutBlock* cb = containingBlock(); cb && !cb->isLayoutView(); cb = cb->containingBlock())
                clipOutPositionedObjects(*clipScope, cb->location(), cb->positionedObjects());  
        clipOutFloatingObjects(rootBlock, *clipScope, rootBlockPhysicalPosition, offsetFromRootBlock);
    }

    GapRects result;

    if (hasTransformRelatedProperty() || style()->columnSpan())
        return result;

    if (childrenInline())
        result = inlineSelectionGaps(rootBlock, rootBlockPhysicalPosition, offsetFromRootBlock, lastLogicalTop, lastLogicalLeft, lastLogicalRight, paintInfo);
    else
        result = blockSelectionGaps(rootBlock, rootBlockPhysicalPosition, offsetFromRootBlock, lastLogicalTop, lastLogicalLeft, lastLogicalRight, paintInfo);

    if (rootBlock == this && (selectionState() != SelectionBoth && selectionState() != SelectionEnd)) {
        result.uniteCenter(blockSelectionGap(rootBlock, rootBlockPhysicalPosition, offsetFromRootBlock,
            lastLogicalTop, lastLogicalLeft, lastLogicalRight, logicalHeight(), paintInfo));
    }
    return result;
}
