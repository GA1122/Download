WTF::String InputHandler::elementText()
{
    if (!isActiveTextEdit())
        return WTF::String();

    return DOMSupport::inputElementText(m_currentFocusElement.get());
}
