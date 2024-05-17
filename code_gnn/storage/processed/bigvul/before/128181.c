Notification::Notification(ExecutionContext* context, const WebNotificationData& data)
    : ActiveDOMObject(context)
    , m_data(data)
    , m_persistentId(kInvalidPersistentId)
    , m_state(NotificationStateIdle)
    , m_asyncRunner(AsyncMethodRunner<Notification>::create(this, &Notification::show))
{
    ASSERT(notificationManager());
}
