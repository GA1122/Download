LayoutUnit RenderFlexibleBox::mainAxisBorderAndPaddingExtentForChild(RenderBox* child) const
{
    return isHorizontalFlow() ? child->borderAndPaddingWidth() : child->borderAndPaddingHeight();
}
