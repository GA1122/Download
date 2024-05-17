void HTMLSelectElement::selectOption(int optionIndex, SelectOptionFlags flags)
{
    bool shouldDeselect = !m_multiple || (flags & DeselectOtherOptions);

    const Vector<HTMLElement*>& items = listItems();
    int listIndex = optionToListIndex(optionIndex);

    HTMLElement* element = 0;
    if (listIndex >= 0) {
        element = items[listIndex];
        if (element->hasTagName(optionTag)) {
            if (m_activeSelectionAnchorIndex < 0 || shouldDeselect)
                setActiveSelectionAnchorIndex(listIndex);
            if (m_activeSelectionEndIndex < 0 || shouldDeselect)
                setActiveSelectionEndIndex(listIndex);
            toHTMLOptionElement(element)->setSelectedState(true);
        }
    }

    if (shouldDeselect)
        deselectItemsWithoutValidation(element);

    if (RenderObject* renderer = this->renderer())
        renderer->updateFromElement();

    scrollToSelection();

    if (usesMenuList()) {
        m_isProcessingUserDrivenChange = flags & UserDriven;
        if (flags & DispatchChangeEvent)
            dispatchChangeEventForMenuList();
        if (RenderObject* renderer = this->renderer()) {
            if (usesMenuList())
                toRenderMenuList(renderer)->didSetSelectedIndex(listIndex);
            else if (renderer->isListBox())
                toRenderListBox(renderer)->selectionChanged();
        }
    }

    setNeedsValidityCheck();
    notifyFormStateChanged();
}
