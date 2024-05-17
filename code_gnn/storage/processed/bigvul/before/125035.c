void RenderFlexibleBox::layoutFlexItems(bool relayoutChildren)
{
    Vector<LineContext> lineContexts;
    OrderedFlexItemList orderedChildren;
    LayoutUnit sumFlexBaseSize;
    double totalFlexGrow;
    double totalWeightedFlexShrink;
    LayoutUnit sumHypotheticalMainSize;

    Vector<LayoutUnit, 16> childSizes;

    m_orderIterator.first();
    LayoutUnit crossAxisOffset = flowAwareBorderBefore() + flowAwarePaddingBefore();
    bool hasInfiniteLineLength = false;
    while (computeNextFlexLine(orderedChildren, sumFlexBaseSize, totalFlexGrow, totalWeightedFlexShrink, sumHypotheticalMainSize, hasInfiniteLineLength, relayoutChildren)) {
        LayoutUnit containerMainInnerSize = mainAxisContentExtent(sumHypotheticalMainSize);
        LayoutUnit availableFreeSpace = containerMainInnerSize - sumFlexBaseSize;
        FlexSign flexSign = (sumHypotheticalMainSize < containerMainInnerSize) ? PositiveFlexibility : NegativeFlexibility;
        InflexibleFlexItemSize inflexibleItems;
        childSizes.reserveCapacity(orderedChildren.size());
        while (!resolveFlexibleLengths(flexSign, orderedChildren, availableFreeSpace, totalFlexGrow, totalWeightedFlexShrink, inflexibleItems, childSizes, hasInfiniteLineLength)) {
            ASSERT(totalFlexGrow >= 0 && totalWeightedFlexShrink >= 0);
            ASSERT(inflexibleItems.size() > 0);
        }

        layoutAndPlaceChildren(crossAxisOffset, orderedChildren, childSizes, availableFreeSpace, relayoutChildren, lineContexts, hasInfiniteLineLength);
    }
    if (hasLineIfEmpty()) {
        LayoutUnit minHeight = borderAndPaddingLogicalHeight()
            + lineHeight(true, isHorizontalWritingMode() ? HorizontalLine : VerticalLine, PositionOfInteriorLineBoxes)
            + scrollbarLogicalHeight();
        if (height() < minHeight)
            setLogicalHeight(minHeight);
    }

    updateLogicalHeight();
    repositionLogicalHeightDependentFlexItems(lineContexts);
}
