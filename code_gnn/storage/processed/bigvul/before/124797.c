LayoutUnit RenderBox::adjustBorderBoxLogicalWidthForBoxSizing(LayoutUnit width) const
{
    LayoutUnit bordersPlusPadding = borderAndPaddingLogicalWidth();
    if (style()->boxSizing() == CONTENT_BOX)
        return width + bordersPlusPadding;
    return max(width, bordersPlusPadding);
}
