WebCore::IntRect InputHandler::boundingBoxForInputField()
{
    if (!isActiveTextEdit())
        return WebCore::IntRect();

    if (!m_currentFocusElement->renderer())
        return WebCore::IntRect();

    if (HTMLInputElement* element = m_currentFocusElement->toInputElement())
        if (element->isSearchField())
            return element->innerBlockElement()->renderer()->absoluteBoundingBoxRect();

    return m_currentFocusElement->renderer()->absoluteBoundingBoxRect();
}
