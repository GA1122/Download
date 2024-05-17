void Notification::close()
{
    if (m_state != NotificationStateShowing)
        return;

    if (m_persistentId == kInvalidPersistentId) {
        executionContext()->postTask(BLINK_FROM_HERE, createSameThreadTask(&Notification::dispatchCloseEvent, this));

        m_state = NotificationStateClosing;
        notificationManager()->close(this);
    } else {
        m_state = NotificationStateClosed;

        SecurityOrigin* origin = executionContext()->securityOrigin();
        ASSERT(origin);

        notificationManager()->closePersistent(WebSecurityOrigin(origin), m_persistentId);
    }
}
