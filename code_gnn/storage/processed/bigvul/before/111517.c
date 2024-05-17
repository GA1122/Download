void InputHandler::setPopupListIndex(int index)
{
    if (index == -2)  
        return clearCurrentFocusElement();

    if (!isActiveSelectPopup())
        return clearCurrentFocusElement();

    RenderObject* renderer = m_currentFocusElement->renderer();
    if (renderer && renderer->isMenuList()) {
        RenderMenuList* renderMenu = toRenderMenuList(renderer);
        renderMenu->hidePopup();
    }

    HTMLSelectElement* selectElement = static_cast<HTMLSelectElement*>(m_currentFocusElement.get());
    int optionIndex = selectElement->listToOptionIndex(index);
    selectElement->optionSelectedByUser(optionIndex, true  , true  );
    clearCurrentFocusElement();
}
