LayoutUnit RenderFlexibleBox::adjustChildSizeForMinAndMax(RenderBox* child, LayoutUnit childSize)
{
    Length max = isHorizontalFlow() ? child->style()->maxWidth() : child->style()->maxHeight();
    if (max.isSpecifiedOrIntrinsic()) {
        LayoutUnit maxExtent = computeMainAxisExtentForChild(child, MaxSize, max);
        if (maxExtent != -1 && childSize > maxExtent)
            childSize = maxExtent;
    }

    Length min = isHorizontalFlow() ? child->style()->minWidth() : child->style()->minHeight();
    LayoutUnit minExtent = 0;
    if (min.isSpecifiedOrIntrinsic())
        minExtent = computeMainAxisExtentForChild(child, MinSize, min);
    return std::max(childSize, minExtent);
}
