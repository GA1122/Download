bool InputHandler::setSelection(int start, int end, bool changeIsPartOfComposition)
{
    if (!isActiveTextEdit())
        return false;

    ASSERT(m_currentFocusElement->document() && m_currentFocusElement->document()->frame());

    ProcessingChangeGuard guard(this);

    VisibleSelection newSelection = DOMSupport::visibleSelectionForRangeInputElement(m_currentFocusElement.get(), start, end);
    m_currentFocusElement->document()->frame()->selection()->setSelection(newSelection, changeIsPartOfComposition ? 0 : FrameSelection::CloseTyping | FrameSelection::ClearTypingStyle);

    InputLog(LogLevelInfo, "InputHandler::setSelection selectionStart=%u, selectionEnd=%u", start, end);

    return start == selectionStart() && end == selectionEnd();
}
