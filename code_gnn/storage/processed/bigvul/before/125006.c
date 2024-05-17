Length RenderFlexibleBox::flexBasisForChild(RenderBox* child) const
{
    Length flexLength = child->style()->flexBasis();
    if (flexLength.isAuto())
        flexLength = isHorizontalFlow() ? child->style()->width() : child->style()->height();
    return flexLength;
}
