void RenderFlexibleBox::layoutAndPlaceChildren(LayoutUnit& crossAxisOffset, const OrderedFlexItemList& children, const Vector<LayoutUnit, 16>& childSizes, LayoutUnit availableFreeSpace, bool relayoutChildren, Vector<LineContext>& lineContexts, bool hasInfiniteLineLength)
{
    ASSERT(childSizes.size() == children.size());

    size_t numberOfChildrenForJustifyContent = numberOfInFlowPositionedChildren(children);
    LayoutUnit autoMarginOffset = autoMarginOffsetInMainAxis(children, availableFreeSpace);
    LayoutUnit mainAxisOffset = flowAwareBorderStart() + flowAwarePaddingStart();
    mainAxisOffset += initialJustifyContentOffset(availableFreeSpace, style()->justifyContent(), numberOfChildrenForJustifyContent);
    if (style()->flexDirection() == FlowRowReverse)
        mainAxisOffset += isHorizontalFlow() ? verticalScrollbarWidth() : horizontalScrollbarHeight();

    LayoutUnit totalMainExtent = mainAxisExtent();
    LayoutUnit maxAscent = 0, maxDescent = 0;  
    LayoutUnit maxChildCrossAxisExtent = 0;
    size_t seenInFlowPositionedChildren = 0;
    bool shouldFlipMainAxis = !isColumnFlow() && !isLeftToRightFlow();
    for (size_t i = 0; i < children.size(); ++i) {
        RenderBox* child = children[i];

        if (child->isOutOfFlowPositioned()) {
            prepareChildForPositionedLayout(child, mainAxisOffset, crossAxisOffset, FlipForRowReverse);
            continue;
        }

        LayoutUnit childPreferredSize = childSizes[i] + mainAxisBorderAndPaddingExtentForChild(child);
        setLogicalOverrideSize(child, childPreferredSize);
        if (childPreferredSize != mainAxisExtentForChild(child)) {
            child->setChildNeedsLayout(MarkOnlyThis);
        } else {
            resetAutoMarginsAndLogicalTopInCrossAxis(child);
        }
        bool forceChildRelayout = relayoutChildren && !childPreferredMainAxisContentExtentRequiresLayout(child, hasInfiniteLineLength);
        updateBlockChildDirtyBitsBeforeLayout(forceChildRelayout, child);
        child->layoutIfNeeded();

        updateAutoMarginsInMainAxis(child, autoMarginOffset);

        LayoutUnit childCrossAxisMarginBoxExtent;
        if (alignmentForChild(child) == ItemPositionBaseline && !hasAutoMarginsInCrossAxis(child)) {
            LayoutUnit ascent = marginBoxAscentForChild(child);
            LayoutUnit descent = (crossAxisMarginExtentForChild(child) + crossAxisExtentForChild(child)) - ascent;

            maxAscent = std::max(maxAscent, ascent);
            maxDescent = std::max(maxDescent, descent);

            childCrossAxisMarginBoxExtent = maxAscent + maxDescent;
        } else {
            childCrossAxisMarginBoxExtent = crossAxisIntrinsicExtentForChild(child) + crossAxisMarginExtentForChild(child);
        }
        if (!isColumnFlow())
            setLogicalHeight(std::max(logicalHeight(), crossAxisOffset + flowAwareBorderAfter() + flowAwarePaddingAfter() + childCrossAxisMarginBoxExtent + crossAxisScrollbarExtent()));
        maxChildCrossAxisExtent = std::max(maxChildCrossAxisExtent, childCrossAxisMarginBoxExtent);

        mainAxisOffset += flowAwareMarginStartForChild(child);

        LayoutUnit childMainExtent = mainAxisExtentForChild(child);
        LayoutPoint childLocation(shouldFlipMainAxis ? totalMainExtent - mainAxisOffset - childMainExtent : mainAxisOffset,
            crossAxisOffset + flowAwareMarginBeforeForChild(child));

        setFlowAwareLocationForChild(child, childLocation);
        mainAxisOffset += childMainExtent + flowAwareMarginEndForChild(child);

        ++seenInFlowPositionedChildren;
        if (seenInFlowPositionedChildren < numberOfChildrenForJustifyContent)
            mainAxisOffset += justifyContentSpaceBetweenChildren(availableFreeSpace, style()->justifyContent(), numberOfChildrenForJustifyContent);
    }

    if (isColumnFlow())
        setLogicalHeight(mainAxisOffset + flowAwareBorderEnd() + flowAwarePaddingEnd() + scrollbarLogicalHeight());

    if (style()->flexDirection() == FlowColumnReverse) {
        updateLogicalHeight();
        layoutColumnReverse(children, crossAxisOffset, availableFreeSpace);
    }

    if (m_numberOfInFlowChildrenOnFirstLine == -1)
        m_numberOfInFlowChildrenOnFirstLine = seenInFlowPositionedChildren;
    lineContexts.append(LineContext(crossAxisOffset, maxChildCrossAxisExtent, children.size(), maxAscent));
    crossAxisOffset += maxChildCrossAxisExtent;
}
