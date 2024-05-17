void InputHandler::setPopupListIndexes(int size, const bool* selecteds)
{
    if (!isActiveSelectPopup())
        return clearCurrentFocusElement();

    if (size < 0)
        return;

    HTMLSelectElement* selectElement = static_cast<HTMLSelectElement*>(m_currentFocusElement.get());
    const WTF::Vector<HTMLElement*>& items = selectElement->listItems();
    if (items.size() != static_cast<unsigned int>(size))
        return;

    HTMLOptionElement* option;
    for (int i = 0; i < size; i++) {
        if (items[i]->hasTagName(HTMLNames::optionTag)) {
            option = static_cast<HTMLOptionElement*>(items[i]);
            option->setSelectedState(selecteds[i]);
        }
    }

    selectElement->dispatchFormControlChangeEvent();
    selectElement->renderer()->repaint();
    clearCurrentFocusElement();
}
