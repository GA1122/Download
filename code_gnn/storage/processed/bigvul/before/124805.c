LayoutUnit RenderBox::availableLogicalHeight(AvailableLogicalHeightType heightType) const
{
    return constrainLogicalHeightByMinMax(availableLogicalHeightUsing(style()->logicalHeight(), heightType), -1);
}
