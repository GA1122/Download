void RenderBox::computePositionedLogicalHeightUsing(Length logicalHeightLength, const RenderBoxModelObject* containerBlock,
                                                    int containerLogicalHeight, int bordersPlusPadding,
                                                    Length logicalTop, Length logicalBottom, Length marginBefore, Length marginAfter,
                                                    int& logicalHeightValue, int& marginBeforeValue, int& marginAfterValue, int& logicalTopPos)
{
    ASSERT(!(logicalTop.isAuto() && logicalBottom.isAuto()));

    int contentLogicalHeight = logicalHeight() - bordersPlusPadding;

    int logicalTopValue = 0;

    bool logicalHeightIsAuto = logicalHeightLength.isAuto();
    bool logicalTopIsAuto = logicalTop.isAuto();
    bool logicalBottomIsAuto = logicalBottom.isAuto();

    if (isTable()) {
        logicalHeightLength.setValue(Fixed, contentLogicalHeight);
        logicalHeightIsAuto = false;
    }

    if (!logicalTopIsAuto && !logicalHeightIsAuto && !logicalBottomIsAuto) {
         

        logicalHeightValue = computeContentBoxLogicalHeight(logicalHeightLength.calcValue(containerLogicalHeight));
        logicalTopValue = logicalTop.calcValue(containerLogicalHeight);

        const int availableSpace = containerLogicalHeight - (logicalTopValue + logicalHeightValue + logicalBottom.calcValue(containerLogicalHeight) + bordersPlusPadding);

        if (marginBefore.isAuto() && marginAfter.isAuto()) {
            marginBeforeValue = availableSpace / 2;  
            marginAfterValue = availableSpace - marginBeforeValue;  
        } else if (marginBefore.isAuto()) {
            marginAfterValue = marginAfter.calcValue(containerLogicalHeight);
            marginBeforeValue = availableSpace - marginAfterValue;
        } else if (marginAfter.isAuto()) {
            marginBeforeValue = marginBefore.calcValue(containerLogicalHeight);
            marginAfterValue = availableSpace - marginBeforeValue;
        } else {
            marginBeforeValue = marginBefore.calcValue(containerLogicalHeight);
            marginAfterValue = marginAfter.calcValue(containerLogicalHeight);
        }
    } else {
         

        marginBeforeValue = marginBefore.calcMinValue(containerLogicalHeight);
        marginAfterValue = marginAfter.calcMinValue(containerLogicalHeight);

        const int availableSpace = containerLogicalHeight - (marginBeforeValue + marginAfterValue + bordersPlusPadding);

        if (logicalTopIsAuto && logicalHeightIsAuto && !logicalBottomIsAuto) {
            logicalHeightValue = contentLogicalHeight;
            logicalTopValue = availableSpace - (logicalHeightValue + logicalBottom.calcValue(containerLogicalHeight));
        } else if (!logicalTopIsAuto && logicalHeightIsAuto && logicalBottomIsAuto) {
            logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
            logicalHeightValue = contentLogicalHeight;
        } else if (logicalTopIsAuto && !logicalHeightIsAuto && !logicalBottomIsAuto) {
            logicalHeightValue = computeContentBoxLogicalHeight(logicalHeightLength.calcValue(containerLogicalHeight));
            logicalTopValue = availableSpace - (logicalHeightValue + logicalBottom.calcValue(containerLogicalHeight));
        } else if (!logicalTopIsAuto && logicalHeightIsAuto && !logicalBottomIsAuto) {
            logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
            logicalHeightValue = max(0, availableSpace - (logicalTopValue + logicalBottom.calcValue(containerLogicalHeight)));
        } else if (!logicalTopIsAuto && !logicalHeightIsAuto && logicalBottomIsAuto) {
            logicalHeightValue = computeContentBoxLogicalHeight(logicalHeightLength.calcValue(containerLogicalHeight));
            logicalTopValue = logicalTop.calcValue(containerLogicalHeight);
        }
    }

    logicalTopPos = logicalTopValue + marginBeforeValue;
    computeLogicalTopPositionedOffset(logicalTopPos, this, logicalHeightValue, containerBlock, containerLogicalHeight);
}
