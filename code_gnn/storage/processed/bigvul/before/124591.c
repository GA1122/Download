LayoutUnit RenderBlock::marginIntrinsicLogicalWidthForChild(RenderBox* child) const
{
    Length marginLeft = child->style()->marginStartUsing(style());
    Length marginRight = child->style()->marginEndUsing(style());
    LayoutUnit margin = 0;
    if (marginLeft.isFixed())
        margin += marginLeft.value();
    if (marginRight.isFixed())
        margin += marginRight.value();
    return margin;
}
