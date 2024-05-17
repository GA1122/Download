 void LayoutBlockFlow::adjustLinePositionForPagination(RootInlineBox& lineBox, LayoutUnit& delta)
{
    LayoutUnit logicalOffset = lineBox.lineTopWithLeading();
    LayoutUnit lineHeight = lineBox.lineBottomWithLeading() - logicalOffset;
    updateMinimumPageHeight(logicalOffset, calculateMinimumPageHeight(styleRef(), lineBox));
    logicalOffset += delta;
    lineBox.setPaginationStrut(LayoutUnit());
    lineBox.setIsFirstAfterPageBreak(false);
    LayoutUnit pageLogicalHeight = pageLogicalHeightForOffset(logicalOffset);
    if (!pageLogicalHeight)
        return;
    if (lineHeight > pageLogicalHeight) {
        return;
    }
    LayoutUnit remainingLogicalHeight = pageRemainingLogicalHeightForOffset(logicalOffset, AssociateWithLatterPage);

    int lineIndex = lineCount(&lineBox);
    if (remainingLogicalHeight < lineHeight || (shouldBreakAtLineToAvoidWidow() && lineBreakToAvoidWidow() == lineIndex)) {
        if (shouldBreakAtLineToAvoidWidow() && lineBreakToAvoidWidow() == lineIndex) {
            clearShouldBreakAtLineToAvoidWidow();
            setDidBreakAtLineToAvoidWidow();
        }
        setPageBreak(logicalOffset, lineHeight - remainingLogicalHeight);
        if (shouldSetStrutOnBlock(*this, lineBox, logicalOffset, lineIndex, remainingLogicalHeight)) {
            LayoutUnit paginationStrut = remainingLogicalHeight + std::max<LayoutUnit>(0, logicalOffset);
            if (isFloating())
                paginationStrut += marginBefore();  
            setPaginationStrut(paginationStrut);
        } else {
            delta += remainingLogicalHeight;
            lineBox.setPaginationStrut(remainingLogicalHeight);
            lineBox.setIsFirstAfterPageBreak(true);
        }
    } else if (remainingLogicalHeight == pageLogicalHeight) {
        if (lineBox != firstRootBox())
            lineBox.setIsFirstAfterPageBreak(true);
        if (lineBox != firstRootBox() || offsetFromLogicalTopOfFirstPage())
            setPageBreak(logicalOffset, lineHeight);
    }
}
