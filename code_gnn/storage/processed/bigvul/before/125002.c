LayoutUnit RenderFlexibleBox::crossAxisIntrinsicExtentForChild(RenderBox* child) const
{
    return isHorizontalFlow() ? childIntrinsicHeight(child) : childIntrinsicWidth(child);
}
