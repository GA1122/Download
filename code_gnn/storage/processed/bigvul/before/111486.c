bool InputHandler::openColorPopup(HTMLInputElement* element)
{
    if (!element || element->isDisabledOrReadOnly() || !DOMSupport::isColorInputField(element))
        return false;

    if (isActiveTextEdit())
        clearCurrentFocusElement();

    m_currentFocusElement = element;
    m_currentFocusElementType = TextPopup;

    m_webPage->m_page->chrome()->client()->closePagePopup(0);

    ColorPickerClient* client = new ColorPickerClient(element->value(), m_webPage, element);
    return m_webPage->m_page->chrome()->client()->openPagePopup(client,  WebCore::IntRect());
}
