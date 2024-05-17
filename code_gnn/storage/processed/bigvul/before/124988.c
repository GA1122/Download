LayoutUnit RenderFlexibleBox::availableAlignmentSpaceForChildBeforeStretching(LayoutUnit lineCrossAxisExtent, RenderBox* child)
{
    ASSERT(!child->isOutOfFlowPositioned());
    LayoutUnit childCrossExtent = crossAxisMarginExtentForChild(child) + crossAxisIntrinsicExtentForChild(child);
    return lineCrossAxisExtent - childCrossExtent;
}
