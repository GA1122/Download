LayoutUnit RenderFlexibleBox::mainAxisExtentForChild(RenderBox* child) const
{
    return isHorizontalFlow() ? child->width() : child->height();
}
