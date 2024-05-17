void HTMLSelectElement::updateListBoxSelection(bool deselectOtherOptions)
{
    ASSERT(renderer() && (renderer()->isListBox() || m_multiple));
    ASSERT(!listItems().size() || m_activeSelectionAnchorIndex >= 0);

    unsigned start = min(m_activeSelectionAnchorIndex, m_activeSelectionEndIndex);
    unsigned end = max(m_activeSelectionAnchorIndex, m_activeSelectionEndIndex);

    const Vector<HTMLElement*>& items = listItems();
    for (unsigned i = 0; i < items.size(); ++i) {
        HTMLElement* element = items[i];
        if (!element->hasTagName(optionTag) || toHTMLOptionElement(element)->isDisabledFormControl())
            continue;

        if (i >= start && i <= end)
            toHTMLOptionElement(element)->setSelectedState(m_activeSelectionState);
        else if (deselectOtherOptions || i >= m_cachedStateForActiveSelection.size())
            toHTMLOptionElement(element)->setSelectedState(false);
        else
            toHTMLOptionElement(element)->setSelectedState(m_cachedStateForActiveSelection[i]);
    }

    scrollToSelection();
    setNeedsValidityCheck();
    notifyFormStateChanged();
}
