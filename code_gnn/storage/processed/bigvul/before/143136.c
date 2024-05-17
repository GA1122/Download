void Document::clearFocusedElementTimerFired(Timer<Document>*)
{
    updateStyleAndLayoutTree();
    m_clearFocusedElementTimer.stop();

    if (m_focusedElement && !m_focusedElement->isFocusable())
        m_focusedElement->blur();
}
