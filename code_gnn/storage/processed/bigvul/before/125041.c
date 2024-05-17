bool RenderFlexibleBox::needToStretchChildLogicalHeight(RenderBox* child) const
{
    if (alignmentForChild(child) != ItemPositionStretch)
        return false;

    return isHorizontalFlow() && child->style()->height().isAuto();
}
