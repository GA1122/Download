void HTMLSelectElement::updateSelectedState(int listIndex, bool multi, bool shift)
{
    ASSERT(listIndex >= 0);

    saveLastSelection();

    m_activeSelectionState = true;

    bool shiftSelect = m_multiple && shift;
    bool multiSelect = m_multiple && multi && !shift;

    HTMLElement* clickedElement = listItems()[listIndex];
    if (clickedElement->hasTagName(optionTag)) {
        if (toHTMLOptionElement(clickedElement)->selected() && multiSelect)
            m_activeSelectionState = false;
        if (!m_activeSelectionState)
            toHTMLOptionElement(clickedElement)->setSelectedState(false);
    }

    if (!shiftSelect && !multiSelect)
        deselectItemsWithoutValidation(clickedElement);

    if (m_activeSelectionAnchorIndex < 0 && !multiSelect)
        setActiveSelectionAnchorIndex(selectedIndex());

    if (clickedElement->hasTagName(optionTag) && !toHTMLOptionElement(clickedElement)->isDisabledFormControl())
        toHTMLOptionElement(clickedElement)->setSelectedState(true);

    if (m_activeSelectionAnchorIndex < 0 || !shiftSelect)
        setActiveSelectionAnchorIndex(listIndex);

    setActiveSelectionEndIndex(listIndex);
    updateListBoxSelection(!multiSelect);
}
