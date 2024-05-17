void Notification::scheduleShow()
{
    ASSERT(m_state == NotificationStateIdle);
    ASSERT(!m_asyncRunner->isActive());

    m_asyncRunner->runAsync();
}
