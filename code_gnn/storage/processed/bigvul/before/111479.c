bool InputHandler::isInputModeEnabled() const
{
    return m_inputModeEnabled || m_webPage->m_dumpRenderTree || Platform::Settings::instance()->alwaysShowKeyboardOnFocus();
}
