void RenderBox::computePositionedLogicalHeight()
{
    if (isReplaced()) {
        computePositionedLogicalHeightReplaced();
        return;
    }



    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());

    const int containerLogicalHeight = containingBlockLogicalHeightForPositioned(containerBlock);

    bool isHorizontal = isHorizontalWritingMode();
    bool isFlipped = style()->isFlippedBlocksWritingMode();
    const int bordersPlusPadding = borderAndPaddingLogicalHeight();
    const Length marginBefore = style()->marginBefore();
    const Length marginAfter = style()->marginAfter();
    int& marginBeforeAlias = isHorizontal ? (isFlipped ? m_marginBottom : m_marginTop) : (isFlipped ? m_marginRight: m_marginLeft);
    int& marginAfterAlias = isHorizontal ? (isFlipped ? m_marginTop : m_marginBottom) : (isFlipped ? m_marginLeft: m_marginRight);

    Length logicalTop = style()->logicalTop();
    Length logicalBottom = style()->logicalBottom();
        
     

    computeBlockStaticDistance(logicalTop, logicalBottom, this, containerBlock);

    int logicalHeightResult;  
    int logicalTopPos;

    computePositionedLogicalHeightUsing(style()->logicalHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding,
                                        logicalTop, logicalBottom, marginBefore, marginAfter,
                                        logicalHeightResult, marginBeforeAlias, marginAfterAlias, logicalTopPos);
    setLogicalTop(logicalTopPos);


    if (!style()->logicalMaxHeight().isUndefined()) {
        int maxLogicalHeight;
        int maxMarginBefore;
        int maxMarginAfter;
        int maxLogicalTopPos;

        computePositionedLogicalHeightUsing(style()->logicalMaxHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding,
                                            logicalTop, logicalBottom, marginBefore, marginAfter,
                                            maxLogicalHeight, maxMarginBefore, maxMarginAfter, maxLogicalTopPos);

        if (logicalHeightResult > maxLogicalHeight) {
            logicalHeightResult = maxLogicalHeight;
            marginBeforeAlias = maxMarginBefore;
            marginAfterAlias = maxMarginAfter;
            setLogicalTop(maxLogicalTopPos);
        }
    }

    if (!style()->logicalMinHeight().isZero()) {
        int minLogicalHeight;
        int minMarginBefore;
        int minMarginAfter;
        int minLogicalTopPos;

        computePositionedLogicalHeightUsing(style()->logicalMinHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding,
                                            logicalTop, logicalBottom, marginBefore, marginAfter,
                                            minLogicalHeight, minMarginBefore, minMarginAfter, minLogicalTopPos);

        if (logicalHeightResult < minLogicalHeight) {
            logicalHeightResult = minLogicalHeight;
            marginBeforeAlias = minMarginBefore;
            marginAfterAlias = minMarginAfter;
            setLogicalTop(minLogicalTopPos);
        }
    }

    setLogicalHeight(logicalHeightResult + bordersPlusPadding);
}
