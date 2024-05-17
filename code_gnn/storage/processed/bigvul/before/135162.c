void Document::clearFocusedElementTimerFired(Timer<Document>*)
{
    updateLayoutTreeIfNeeded();
    m_clearFocusedElementTimer.stop();

    if (m_focusedElement && !m_focusedElement->isFocusable())
        m_focusedElement->blur();
}
