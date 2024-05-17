void InputHandler::selectionChanged()
{
    if (!m_webPage->m_mainFrame)
        return;

    if (!isActiveTextEdit())
        return;

    if (processingChange())
        return;

    if (m_shouldEnsureFocusTextElementVisibleOnSelectionChanged) {
        ensureFocusTextElementVisible(EdgeIfNeeded);
        m_shouldEnsureFocusTextElementVisibleOnSelectionChanged = false;
    }

    ASSERT(m_currentFocusElement->document() && m_currentFocusElement->document()->frame());

    if (m_receivedBackspaceKeyDown)
        return;

    int newSelectionStart = selectionStart();
    int newSelectionEnd = selectionEnd();

    InputLog(LogLevelInfo, "InputHandler::selectionChanged selectionStart=%u, selectionEnd=%u", newSelectionStart, newSelectionEnd);

    m_webPage->m_client->inputSelectionChanged(newSelectionStart, newSelectionEnd);

    removeAttributedTextMarker();
}
