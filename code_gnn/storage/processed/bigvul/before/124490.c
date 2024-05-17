void RenderBlock::adjustLinePositionForPagination(RootInlineBox* lineBox, LayoutUnit& delta, RenderFlowThread* flowThread)
{
    LayoutRect logicalVisualOverflow = lineBox->logicalVisualOverflowRect(lineBox->lineTop(), lineBox->lineBottom());
    LayoutUnit logicalOffset = min(lineBox->lineTopWithLeading(), logicalVisualOverflow.y());
    LayoutUnit logicalBottom = max(lineBox->lineBottomWithLeading(), logicalVisualOverflow.maxY());
    LayoutUnit lineHeight = logicalBottom - logicalOffset;
    updateMinimumPageHeight(logicalOffset, calculateMinimumPageHeight(style(), lineBox, logicalOffset, logicalBottom));
    logicalOffset += delta;
    lineBox->setPaginationStrut(0);
    lineBox->setIsFirstAfterPageBreak(false);
    LayoutUnit pageLogicalHeight = pageLogicalHeightForOffset(logicalOffset);
    bool hasUniformPageLogicalHeight = !flowThread || flowThread->regionsHaveUniformLogicalHeight();
    if (!pageLogicalHeight || (hasUniformPageLogicalHeight && logicalVisualOverflow.height() > pageLogicalHeight))
        return;
    LayoutUnit remainingLogicalHeight = pageRemainingLogicalHeightForOffset(logicalOffset, ExcludePageBoundary);

    int lineIndex = lineCount(lineBox);
    if (remainingLogicalHeight < lineHeight || (shouldBreakAtLineToAvoidWidow() && lineBreakToAvoidWidow() == lineIndex)) {
        if (shouldBreakAtLineToAvoidWidow() && lineBreakToAvoidWidow() == lineIndex) {
            clearShouldBreakAtLineToAvoidWidow();
            setDidBreakAtLineToAvoidWidow();
        }
        if (lineHeight > pageLogicalHeight) {
            remainingLogicalHeight -= min(lineHeight - pageLogicalHeight, max<LayoutUnit>(0, logicalVisualOverflow.y() - lineBox->lineTopWithLeading()));
        }
        LayoutUnit totalLogicalHeight = lineHeight + max<LayoutUnit>(0, logicalOffset);
        LayoutUnit pageLogicalHeightAtNewOffset = hasUniformPageLogicalHeight ? pageLogicalHeight : pageLogicalHeightForOffset(logicalOffset + remainingLogicalHeight);
        setPageBreak(logicalOffset, lineHeight - remainingLogicalHeight);
        if (((lineBox == firstRootBox() && totalLogicalHeight < pageLogicalHeightAtNewOffset) || (!style()->hasAutoOrphans() && style()->orphans() >= lineIndex))
            && !isOutOfFlowPositioned() && !isTableCell())
            setPaginationStrut(remainingLogicalHeight + max<LayoutUnit>(0, logicalOffset));
        else {
            delta += remainingLogicalHeight;
            lineBox->setPaginationStrut(remainingLogicalHeight);
            lineBox->setIsFirstAfterPageBreak(true);
        }
    } else if (remainingLogicalHeight == pageLogicalHeight) {
        if (lineBox != firstRootBox())
            lineBox->setIsFirstAfterPageBreak(true);
        if (lineBox != firstRootBox() || offsetFromLogicalTopOfFirstPage())
            setPageBreak(logicalOffset, lineHeight);
    }
}
