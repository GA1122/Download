LayoutUnit RenderFlexibleBox::childIntrinsicWidth(RenderBox* child) const
{
    if (!child->isHorizontalWritingMode() && needToStretchChildLogicalHeight(child))
        return constrainedChildIntrinsicContentLogicalHeight(child);
    return child->width();
}
