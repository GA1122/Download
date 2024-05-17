LayoutUnit RenderBox::computeLogicalWidthUsing(SizeType widthType, Length logicalWidth, LayoutUnit availableLogicalWidth, const RenderBlock* cb) const
{
    if (!logicalWidth.isIntrinsicOrAuto()) {
        return adjustBorderBoxLogicalWidthForBoxSizing(valueForLength(logicalWidth, availableLogicalWidth));
    }

    if (logicalWidth.isIntrinsic())
        return computeIntrinsicLogicalWidthUsing(logicalWidth, availableLogicalWidth, borderAndPaddingLogicalWidth());

    LayoutUnit marginStart = 0;
    LayoutUnit marginEnd = 0;
    LayoutUnit logicalWidthResult = fillAvailableMeasure(availableLogicalWidth, marginStart, marginEnd);

    if (shrinkToAvoidFloats() && cb->containsFloats())
        logicalWidthResult = min(logicalWidthResult, shrinkLogicalWidthToAvoidFloats(marginStart, marginEnd, toRenderBlockFlow(cb)));

    if (widthType == MainOrPreferredSize && sizesLogicalWidthToFitContent(logicalWidth))
        return max(minPreferredLogicalWidth(), min(maxPreferredLogicalWidth(), logicalWidthResult));
    return logicalWidthResult;
}
