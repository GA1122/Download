LayoutUnit RenderBox::adjustContentBoxLogicalHeightForBoxSizing(LayoutUnit height) const
{
    if (style()->boxSizing() == BORDER_BOX)
        height -= borderAndPaddingLogicalHeight();
    return max<LayoutUnit>(0, height);
}
