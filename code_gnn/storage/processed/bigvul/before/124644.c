bool RenderBlock::shouldPaintSelectionGaps() const
{
    return selectionState() != SelectionNone && style()->visibility() == VISIBLE && isSelectionRoot();
}
