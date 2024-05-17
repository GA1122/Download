void InputHandler::setInputModeEnabled(bool active)
{
    FocusLog(LogLevelInfo, "InputHandler::setInputModeEnabled '%s', override is '%s'"
             , active ? "true" : "false"
             , m_webPage->m_dumpRenderTree || Platform::Settings::instance()->alwaysShowKeyboardOnFocus() ? "true" : "false");

    m_inputModeEnabled = active;

    if (isInputModeEnabled() && isActiveTextEdit() && !m_currentFocusElement->document()->frame()->selection()->isFocused())
        m_currentFocusElement->document()->frame()->selection()->setFocused(true);
}
