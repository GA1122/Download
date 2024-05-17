void RenderBox::computePositionedLogicalHeight(LogicalExtentComputedValues& computedValues) const
{
    if (isReplaced()) {
        computePositionedLogicalHeightReplaced(computedValues);
        return;
    }



    const RenderBoxModelObject* containerBlock = toRenderBoxModelObject(container());

    const LayoutUnit containerLogicalHeight = containingBlockLogicalHeightForPositioned(containerBlock);

    RenderStyle* styleToUse = style();
    const LayoutUnit bordersPlusPadding = borderAndPaddingLogicalHeight();
    const Length marginBefore = styleToUse->marginBefore();
    const Length marginAfter = styleToUse->marginAfter();
    Length logicalTopLength = styleToUse->logicalTop();
    Length logicalBottomLength = styleToUse->logicalBottom();

     

    computeBlockStaticDistance(logicalTopLength, logicalBottomLength, this, containerBlock);

    LayoutUnit logicalHeight = computedValues.m_extent;
    computePositionedLogicalHeightUsing(styleToUse->logicalHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding, logicalHeight,
                                        logicalTopLength, logicalBottomLength, marginBefore, marginAfter,
                                        computedValues);


    if (!styleToUse->logicalMaxHeight().isUndefined()) {
        LogicalExtentComputedValues maxValues;

        computePositionedLogicalHeightUsing(styleToUse->logicalMaxHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding, logicalHeight,
                                            logicalTopLength, logicalBottomLength, marginBefore, marginAfter,
                                            maxValues);

        if (computedValues.m_extent > maxValues.m_extent) {
            computedValues.m_extent = maxValues.m_extent;
            computedValues.m_position = maxValues.m_position;
            computedValues.m_margins.m_before = maxValues.m_margins.m_before;
            computedValues.m_margins.m_after = maxValues.m_margins.m_after;
        }
    }

    if (!styleToUse->logicalMinHeight().isZero() || styleToUse->logicalMinHeight().isIntrinsic()) {
        LogicalExtentComputedValues minValues;

        computePositionedLogicalHeightUsing(styleToUse->logicalMinHeight(), containerBlock, containerLogicalHeight, bordersPlusPadding, logicalHeight,
                                            logicalTopLength, logicalBottomLength, marginBefore, marginAfter,
                                            minValues);

        if (computedValues.m_extent < minValues.m_extent) {
            computedValues.m_extent = minValues.m_extent;
            computedValues.m_position = minValues.m_position;
            computedValues.m_margins.m_before = minValues.m_margins.m_before;
            computedValues.m_margins.m_after = minValues.m_margins.m_after;
        }
    }

    computedValues.m_extent += bordersPlusPadding;
}
