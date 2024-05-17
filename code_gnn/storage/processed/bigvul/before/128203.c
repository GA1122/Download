void Notification::show()
{
    ASSERT(m_state == NotificationStateIdle);
    if (Notification::checkPermission(executionContext()) != WebNotificationPermissionAllowed) {
        dispatchErrorEvent();
        return;
    }

    SecurityOrigin* origin = executionContext()->securityOrigin();
    ASSERT(origin);

    notificationManager()->show(WebSecurityOrigin(origin), m_data, this);

    m_state = NotificationStateShowing;
}
