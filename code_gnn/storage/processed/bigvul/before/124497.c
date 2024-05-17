LayoutUnit RenderBlock::availableLogicalWidth() const
{
    if (hasColumns())
        return desiredColumnWidth();
    return RenderBox::availableLogicalWidth();
}
