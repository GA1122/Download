void RenderFlexibleBox::layoutColumnReverse(const OrderedFlexItemList& children, LayoutUnit crossAxisOffset, LayoutUnit availableFreeSpace)
{
    size_t numberOfChildrenForJustifyContent = numberOfInFlowPositionedChildren(children);
    LayoutUnit mainAxisOffset = logicalHeight() - flowAwareBorderEnd() - flowAwarePaddingEnd();
    mainAxisOffset -= initialJustifyContentOffset(availableFreeSpace, style()->justifyContent(), numberOfChildrenForJustifyContent);
    mainAxisOffset -= isHorizontalFlow() ? verticalScrollbarWidth() : horizontalScrollbarHeight();

    size_t seenInFlowPositionedChildren = 0;
    for (size_t i = 0; i < children.size(); ++i) {
        RenderBox* child = children[i];

        if (child->isOutOfFlowPositioned()) {
            child->layer()->setStaticBlockPosition(mainAxisOffset);
            continue;
        }
        mainAxisOffset -= mainAxisExtentForChild(child) + flowAwareMarginEndForChild(child);

        setFlowAwareLocationForChild(child, LayoutPoint(mainAxisOffset, crossAxisOffset + flowAwareMarginBeforeForChild(child)));

        mainAxisOffset -= flowAwareMarginStartForChild(child);

        ++seenInFlowPositionedChildren;
        if (seenInFlowPositionedChildren < numberOfChildrenForJustifyContent)
            mainAxisOffset -= justifyContentSpaceBetweenChildren(availableFreeSpace, style()->justifyContent(), numberOfChildrenForJustifyContent);
    }
}
