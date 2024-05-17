void InputHandler::setPluginFocused(Element* element)
{
    ASSERT(DOMSupport::isElementTypePlugin(element));

    if (isActiveTextEdit())
        setElementUnfocused();

    m_currentFocusElementType = Plugin;
    m_currentFocusElement = element;
}
