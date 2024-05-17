int HTMLSelectElement::nextSelectableListIndexPageAway(int startIndex, SkipDirection direction) const
{
    const Vector<HTMLElement*>& items = listItems();
    int pageSize = 0;
    if (renderer()->isListBox())
        pageSize = toRenderListBox(renderer())->size() - 1;  

    int edgeIndex = (direction == SkipForwards) ? 0 : (items.size() - 1);
    int skipAmount = pageSize + ((direction == SkipForwards) ? startIndex : (edgeIndex - startIndex));
    return nextValidIndex(edgeIndex, direction, skipAmount);
}
