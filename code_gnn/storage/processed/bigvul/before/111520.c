bool InputHandler::setRelativeCursorPosition(int insertionPoint, int relativeCursorPosition)
{
    if (!isActiveTextEdit())
        return false;

    if (relativeCursorPosition == 1) {
        m_currentFocusElement->document()->frame()->selection()->revealSelection(ScrollAlignment::alignToEdgeIfNeeded);
        return true;
    }

    int cursorPosition = 0;
    if (relativeCursorPosition <= 0) {
        cursorPosition = insertionPoint + relativeCursorPosition;
    } else {
        cursorPosition = caretPosition() + relativeCursorPosition - 1;
    }

    if (cursorPosition < 0 || cursorPosition > (int)elementText().length())
        return false;

    InputLog(LogLevelInfo, "InputHandler::setRelativeCursorPosition cursor position %d", cursorPosition);

    return setCursorPosition(cursorPosition);
}
