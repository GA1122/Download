bool LayoutBlockFlow::shouldPaintSelectionGaps() const
{
    if (RuntimeEnabledFeatures::selectionPaintingWithoutSelectionGapsEnabled())
        return false;
    return selectionState() != SelectionNone && style()->visibility() == VISIBLE && isSelectionRoot();
}
