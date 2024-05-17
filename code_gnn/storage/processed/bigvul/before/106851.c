void RenderBox::computePositionedLogicalWidthReplaced()
{

    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());

    const int containerLogicalWidth = containingBlockLogicalWidthForPositioned(containerBlock);

    TextDirection containerDirection = (document()->inQuirksMode()) ? parent()->style()->direction() : containerBlock->style()->direction();

    bool isHorizontal = isHorizontalWritingMode();
    Length logicalLeft = style()->logicalLeft();
    Length logicalRight = style()->logicalRight();
    Length marginLogicalLeft = isHorizontal ? style()->marginLeft() : style()->marginTop();
    Length marginLogicalRight = isHorizontal ? style()->marginRight() : style()->marginBottom();
    int& marginLogicalLeftAlias = isHorizontal ? m_marginLeft : m_marginTop;
    int& marginLogicalRightAlias = isHorizontal ? m_marginRight : m_marginBottom;

     
    setLogicalWidth(computeReplacedLogicalWidth() + borderAndPaddingLogicalWidth());
    const int availableSpace = containerLogicalWidth - logicalWidth();

     
    computeInlineStaticDistance(logicalLeft, logicalRight, this, containerBlock, containerLogicalWidth, containerDirection);

     
    if (logicalLeft.isAuto() || logicalRight.isAuto()) {
        if (marginLogicalLeft.isAuto())
            marginLogicalLeft.setValue(Fixed, 0);
        if (marginLogicalRight.isAuto())
            marginLogicalRight.setValue(Fixed, 0);
    }

     
    int logicalLeftValue = 0;
    int logicalRightValue = 0;

    if (marginLogicalLeft.isAuto() && marginLogicalRight.isAuto()) {
        ASSERT(!(logicalLeft.isAuto() && logicalRight.isAuto()));

        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        int difference = availableSpace - (logicalLeftValue + logicalRightValue);
        if (difference > 0) {
            marginLogicalLeftAlias = difference / 2;  
            marginLogicalRightAlias = difference - marginLogicalLeftAlias;  
        } else {
            if (containerDirection == LTR) {
                marginLogicalLeftAlias = 0;
                marginLogicalRightAlias = difference;  
            } else {
                marginLogicalLeftAlias = difference;  
                marginLogicalRightAlias = 0;
            }
        }

     
    } else if (logicalLeft.isAuto()) {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        logicalLeftValue = availableSpace - (logicalRightValue + marginLogicalLeftAlias + marginLogicalRightAlias);
    } else if (logicalRight.isAuto()) {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);

        logicalRightValue = availableSpace - (logicalLeftValue + marginLogicalLeftAlias + marginLogicalRightAlias);
    } else if (marginLogicalLeft.isAuto()) {
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        marginLogicalLeftAlias = availableSpace - (logicalLeftValue + logicalRightValue + marginLogicalRightAlias);
    } else if (marginLogicalRight.isAuto()) {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

        marginLogicalRightAlias = availableSpace - (logicalLeftValue + logicalRightValue + marginLogicalLeftAlias);
    } else {
        marginLogicalLeftAlias = marginLogicalLeft.calcValue(containerLogicalWidth);
        marginLogicalRightAlias = marginLogicalRight.calcValue(containerLogicalWidth);
        logicalRightValue = logicalRight.calcValue(containerLogicalWidth);
        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
    }

     
    int totalLogicalWidth = logicalWidth() + logicalLeftValue + logicalRightValue +  marginLogicalLeftAlias + marginLogicalRightAlias;
    if (totalLogicalWidth > containerLogicalWidth && (containerDirection == RTL))
        logicalLeftValue = containerLogicalWidth - (totalLogicalWidth - logicalLeftValue);

    

    if (containerBlock->isRenderInline() && !containerBlock->style()->isLeftToRightDirection()) {
        const RenderInline* flow = toRenderInline(containerBlock);
        InlineFlowBox* firstLine = flow->firstLineBox();
        InlineFlowBox* lastLine = flow->lastLineBox();
        if (firstLine && lastLine && firstLine != lastLine) {
            setLogicalLeft(logicalLeftValue + marginLogicalLeftAlias + lastLine->borderLogicalLeft() + (lastLine->logicalLeft() - firstLine->logicalLeft()));
            return;
        }
    }

    int logicalLeftPos = logicalLeftValue + marginLogicalLeftAlias;
    computeLogicalLeftPositionedOffset(logicalLeftPos, this, logicalWidth(), containerBlock, containerLogicalWidth);    
    setLogicalLeft(logicalLeftPos);
}
