void RenderBox::computePositionedLogicalWidthUsing(Length logicalWidth, const RenderBoxModelObject* containerBlock, TextDirection containerDirection,
                                                   int containerLogicalWidth, int bordersPlusPadding,
                                                   Length logicalLeft, Length logicalRight, Length marginLogicalLeft, Length marginLogicalRight,
                                                   int& logicalWidthValue, int& marginLogicalLeftValue, int& marginLogicalRightValue, int& logicalLeftPos)
{
    ASSERT(!(logicalLeft.isAuto() && logicalRight.isAuto()));

    int logicalLeftValue = 0;

    bool logicalWidthIsAuto = logicalWidth.isIntrinsicOrAuto();
    bool logicalLeftIsAuto = logicalLeft.isAuto();
    bool logicalRightIsAuto = logicalRight.isAuto();

    if (!logicalLeftIsAuto && !logicalWidthIsAuto && !logicalRightIsAuto) {
         

        logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
        logicalWidthValue = computeContentBoxLogicalWidth(logicalWidth.calcValue(containerLogicalWidth));

        const int availableSpace = containerLogicalWidth - (logicalLeftValue + logicalWidthValue + logicalRight.calcValue(containerLogicalWidth) + bordersPlusPadding);

        if (marginLogicalLeft.isAuto() && marginLogicalRight.isAuto()) {
            if (availableSpace >= 0) {
                marginLogicalLeftValue = availableSpace / 2;  
                marginLogicalRightValue = availableSpace - marginLogicalLeftValue;  
            } else {
                if (containerDirection == LTR) {
                    marginLogicalLeftValue = 0;
                    marginLogicalRightValue = availableSpace;  
                } else {
                    marginLogicalLeftValue = availableSpace;  
                    marginLogicalRightValue = 0;
                }
            }
        } else if (marginLogicalLeft.isAuto()) {
            marginLogicalRightValue = marginLogicalRight.calcValue(containerLogicalWidth);
            marginLogicalLeftValue = availableSpace - marginLogicalRightValue;
        } else if (marginLogicalRight.isAuto()) {
            marginLogicalLeftValue = marginLogicalLeft.calcValue(containerLogicalWidth);
            marginLogicalRightValue = availableSpace - marginLogicalLeftValue;
        } else {
            marginLogicalLeftValue = marginLogicalLeft.calcValue(containerLogicalWidth);
            marginLogicalRightValue = marginLogicalRight.calcValue(containerLogicalWidth);

            if (containerDirection == RTL)
                logicalLeftValue = (availableSpace + logicalLeftValue) - marginLogicalLeftValue - marginLogicalRightValue;
        }
    } else {
         

        marginLogicalLeftValue = marginLogicalLeft.calcMinValue(containerLogicalWidth);
        marginLogicalRightValue = marginLogicalRight.calcMinValue(containerLogicalWidth);

        const int availableSpace = containerLogicalWidth - (marginLogicalLeftValue + marginLogicalRightValue + bordersPlusPadding);

        if (logicalLeftIsAuto && logicalWidthIsAuto && !logicalRightIsAuto) {
            int logicalRightValue = logicalRight.calcValue(containerLogicalWidth);

            int preferredWidth = maxPreferredLogicalWidth() - bordersPlusPadding;
            int preferredMinWidth = minPreferredLogicalWidth() - bordersPlusPadding;
            int availableWidth = availableSpace - logicalRightValue;
            logicalWidthValue = min(max(preferredMinWidth, availableWidth), preferredWidth);
            logicalLeftValue = availableSpace - (logicalWidthValue + logicalRightValue);
        } else if (!logicalLeftIsAuto && logicalWidthIsAuto && logicalRightIsAuto) {
            logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);

            int preferredWidth = maxPreferredLogicalWidth() - bordersPlusPadding;
            int preferredMinWidth = minPreferredLogicalWidth() - bordersPlusPadding;
            int availableWidth = availableSpace - logicalLeftValue;
            logicalWidthValue = min(max(preferredMinWidth, availableWidth), preferredWidth);
        } else if (logicalLeftIsAuto && !logicalWidthIsAuto && !logicalRightIsAuto) {
            logicalWidthValue = computeContentBoxLogicalWidth(logicalWidth.calcValue(containerLogicalWidth));
            logicalLeftValue = availableSpace - (logicalWidthValue + logicalRight.calcValue(containerLogicalWidth));
        } else if (!logicalLeftIsAuto && logicalWidthIsAuto && !logicalRightIsAuto) {
            logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
            logicalWidthValue = availableSpace - (logicalLeftValue + logicalRight.calcValue(containerLogicalWidth));
        } else if (!logicalLeftIsAuto && !logicalWidthIsAuto && logicalRightIsAuto) {
            logicalLeftValue = logicalLeft.calcValue(containerLogicalWidth);
            logicalWidthValue = computeContentBoxLogicalWidth(logicalWidth.calcValue(containerLogicalWidth));
        }
    }


    if (containerBlock->isRenderInline() && !containerBlock->style()->isLeftToRightDirection()) {
        const RenderInline* flow = toRenderInline(containerBlock);
        InlineFlowBox* firstLine = flow->firstLineBox();
        InlineFlowBox* lastLine = flow->lastLineBox();
        if (firstLine && lastLine && firstLine != lastLine) {
            logicalLeftPos = logicalLeftValue + marginLogicalLeftValue + lastLine->borderLogicalLeft() + (lastLine->logicalLeft() - firstLine->logicalLeft());
            return;
        }
    }

    logicalLeftPos = logicalLeftValue + marginLogicalLeftValue;
    computeLogicalLeftPositionedOffset(logicalLeftPos, this, logicalWidthValue, containerBlock, containerLogicalWidth);
}
