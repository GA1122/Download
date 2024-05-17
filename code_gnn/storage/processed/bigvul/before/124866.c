LayoutUnit RenderBox::fillAvailableMeasure(LayoutUnit availableLogicalWidth, LayoutUnit& marginStart, LayoutUnit& marginEnd) const
{
    marginStart = minimumValueForLength(style()->marginStart(), availableLogicalWidth);
    marginEnd = minimumValueForLength(style()->marginEnd(), availableLogicalWidth);
    return availableLogicalWidth - marginStart - marginEnd;
}
