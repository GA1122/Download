void InputHandler::setProcessingChange(bool processingChange)
{
    if (processingChange == m_processingChange)
        return;

    m_processingChange = processingChange;

    if (!m_processingChange)
        m_webPage->m_selectionHandler->inputHandlerDidFinishProcessingChange();
}
