EventQueue* Document::getEventQueue() const
{
    if (!m_domWindow)
        return 0;
    return m_domWindow->getEventQueue();
}
