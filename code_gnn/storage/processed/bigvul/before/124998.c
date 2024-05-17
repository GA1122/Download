static inline LayoutUnit constrainedChildIntrinsicContentLogicalHeight(RenderBox* child)
{
    LayoutUnit childIntrinsicContentLogicalHeight = child->intrinsicContentLogicalHeight();
    return child->constrainLogicalHeightByMinMax(childIntrinsicContentLogicalHeight + child->borderAndPaddingLogicalHeight(), childIntrinsicContentLogicalHeight);
}
