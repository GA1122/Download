void InputHandler::processPendingKeyboardVisibilityChange()
{
    if (!m_delayKeyboardVisibilityChange) {
        ASSERT(m_pendingKeyboardVisibilityChange == NoChange);
        return;
    }

    m_delayKeyboardVisibilityChange = false;

    if (m_pendingKeyboardVisibilityChange == NoChange)
        return;

    notifyClientOfKeyboardVisibilityChange(m_pendingKeyboardVisibilityChange == Visible);
    m_pendingKeyboardVisibilityChange = NoChange;
}
