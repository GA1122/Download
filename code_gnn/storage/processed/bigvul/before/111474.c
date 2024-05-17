void InputHandler::handleInputLocaleChanged(bool isRTL)
{
    if (!isActiveTextEdit())
        return;

    ASSERT(m_currentFocusElement->document() && m_currentFocusElement->document()->frame());
    RenderObject* renderer = m_currentFocusElement->renderer();
    if (!renderer)
        return;

    Editor* editor = m_currentFocusElement->document()->frame()->editor();
    ASSERT(editor);
    if ((renderer->style()->direction() == RTL) != isRTL)
        editor->setBaseWritingDirection(isRTL ? RightToLeftWritingDirection : LeftToRightWritingDirection);
}
