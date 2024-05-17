void RenderBox::computePositionedLogicalWidth()
{
    if (isReplaced()) {
        computePositionedLogicalWidthReplaced();
        return;
    }






    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());
    
    const int containerLogicalWidth = containingBlockLogicalWidthForPositioned(containerBlock);

    TextDirection containerDirection = (document()->inQuirksMode()) ? parent()->style()->direction() : containerBlock->style()->direction();

    bool isHorizontal = isHorizontalWritingMode();
    const int bordersPlusPadding = borderAndPaddingLogicalWidth();
    const Length marginLogicalLeft = isHorizontal ? style()->marginLeft() : style()->marginTop();
    const Length marginLogicalRight = isHorizontal ? style()->marginRight() : style()->marginBottom();
    int& marginLogicalLeftAlias = isHorizontal ? m_marginLeft : m_marginTop;
    int& marginLogicalRightAlias = isHorizontal ? m_marginRight : m_marginBottom;

    Length logicalLeft = style()->logicalLeft();
    Length logicalRight = style()->logicalRight();

     

    computeInlineStaticDistance(logicalLeft, logicalRight, this, containerBlock, containerLogicalWidth, containerDirection);
    
    int logicalWidthResult;
    int logicalLeftResult;
    computePositionedLogicalWidthUsing(style()->logicalWidth(), containerBlock, containerDirection,
                                       containerLogicalWidth, bordersPlusPadding,
                                       logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                       logicalWidthResult, marginLogicalLeftAlias, marginLogicalRightAlias, logicalLeftResult);
    setLogicalWidth(logicalWidthResult);
    setLogicalLeft(logicalLeftResult);

    if (!style()->logicalMaxWidth().isUndefined()) {
        int maxLogicalWidth;
        int maxMarginLogicalLeft;
        int maxMarginLogicalRight;
        int maxLogicalLeftPos;

        computePositionedLogicalWidthUsing(style()->logicalMaxWidth(), containerBlock, containerDirection,
                                           containerLogicalWidth, bordersPlusPadding,
                                           logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                           maxLogicalWidth, maxMarginLogicalLeft, maxMarginLogicalRight, maxLogicalLeftPos);

        if (logicalWidth() > maxLogicalWidth) {
            setLogicalWidth(maxLogicalWidth);
            marginLogicalLeftAlias = maxMarginLogicalLeft;
            marginLogicalRightAlias = maxMarginLogicalRight;
            setLogicalLeft(maxLogicalLeftPos);
        }
    }

    if (!style()->logicalMinWidth().isZero()) {
        int minLogicalWidth;
        int minMarginLogicalLeft;
        int minMarginLogicalRight;
        int minLogicalLeftPos;

        computePositionedLogicalWidthUsing(style()->logicalMinWidth(), containerBlock, containerDirection,
                                           containerLogicalWidth, bordersPlusPadding,
                                           logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                           minLogicalWidth, minMarginLogicalLeft, minMarginLogicalRight, minLogicalLeftPos);

        if (logicalWidth() < minLogicalWidth) {
            setLogicalWidth(minLogicalWidth);
            marginLogicalLeftAlias = minMarginLogicalLeft;
            marginLogicalRightAlias = minMarginLogicalRight;
            setLogicalLeft(minLogicalLeftPos);
        }
    }

    if (stretchesToMinIntrinsicLogicalWidth() && logicalWidth() < minPreferredLogicalWidth() - bordersPlusPadding) {
        computePositionedLogicalWidthUsing(Length(minPreferredLogicalWidth() - bordersPlusPadding, Fixed), containerBlock, containerDirection,
                                           containerLogicalWidth, bordersPlusPadding,
                                           logicalLeft, logicalRight, marginLogicalLeft, marginLogicalRight,
                                           logicalWidthResult, marginLogicalLeftAlias, marginLogicalRightAlias, logicalLeftResult);
        setLogicalWidth(logicalWidthResult);
        setLogicalLeft(logicalLeftResult);
    }

    setLogicalWidth(logicalWidth() + bordersPlusPadding);
}
