LayoutUnit RenderBox::computeLogicalWidthUsing(LogicalWidthType widthType, LayoutUnit availableLogicalWidth)
{
    LayoutUnit logicalWidthResult = logicalWidth();
    Length logicalWidth;
    if (widthType == LogicalWidth)
        logicalWidth = style()->logicalWidth();
    else if (widthType == MinLogicalWidth)
        logicalWidth = style()->logicalMinWidth();
    else
        logicalWidth = style()->logicalMaxWidth();

    if (logicalWidth.isIntrinsicOrAuto()) {
        LayoutUnit marginStart = style()->marginStart().calcMinValue(availableLogicalWidth);
        LayoutUnit marginEnd = style()->marginEnd().calcMinValue(availableLogicalWidth);
        if (availableLogicalWidth)
            logicalWidthResult = availableLogicalWidth - marginStart - marginEnd;

        if (sizesToIntrinsicLogicalWidth(widthType)) {
            logicalWidthResult = max(logicalWidthResult, minPreferredLogicalWidth());
            logicalWidthResult = min(logicalWidthResult, maxPreferredLogicalWidth());
        }
    } else  
        logicalWidthResult = computeBorderBoxLogicalWidth(logicalWidth.calcValue(availableLogicalWidth)); 

    return logicalWidthResult;
}
