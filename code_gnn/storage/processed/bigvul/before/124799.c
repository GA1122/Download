LayoutUnit RenderBox::adjustContentBoxLogicalWidthForBoxSizing(LayoutUnit width) const
{
    if (style()->boxSizing() == BORDER_BOX)
        width -= borderAndPaddingLogicalWidth();
    return max<LayoutUnit>(0, width);
}
