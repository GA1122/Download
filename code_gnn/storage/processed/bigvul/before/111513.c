void InputHandler::setElementUnfocused(bool refocusOccuring)
{
    if (isActiveTextEdit()) {
        FocusLog(LogLevelInfo, "InputHandler::setElementUnfocused");

        learnText();

        finishComposition();

        if (!refocusOccuring)
            notifyClientOfKeyboardVisibilityChange(false, true  );

        m_webPage->m_client->inputFocusLost();

        if (!m_currentFocusElement->document()->frame()->selection()->isFocused())
            m_currentFocusElement->document()->frame()->selection()->setFocused(true);
    }

    m_currentFocusElement = 0;
    m_currentFocusElementType = TextEdit;
}
