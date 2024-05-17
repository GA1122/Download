void RenderFlexibleBox::setLogicalOverrideSize(RenderBox* child, LayoutUnit childPreferredSize)
{
    if (hasOrthogonalFlow(child))
        child->setOverrideLogicalContentHeight(childPreferredSize - child->borderAndPaddingLogicalHeight());
    else
        child->setOverrideLogicalContentWidth(childPreferredSize - child->borderAndPaddingLogicalWidth());
}
