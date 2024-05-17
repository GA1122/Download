LayoutUnit RenderFlexibleBox::childIntrinsicHeight(RenderBox* child) const
{
    if (child->isHorizontalWritingMode() && needToStretchChildLogicalHeight(child))
        return constrainedChildIntrinsicContentLogicalHeight(child);
    return child->height();
}
