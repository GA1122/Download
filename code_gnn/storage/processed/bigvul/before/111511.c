void InputHandler::setDelayKeyboardVisibilityChange(bool value)
{
    m_delayKeyboardVisibilityChange = value;
    m_pendingKeyboardVisibilityChange = NoChange;
}
