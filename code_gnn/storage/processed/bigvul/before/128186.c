Notification* Notification::create(ExecutionContext* context, int64_t persistentId, const WebNotificationData& data, bool showing)
{
    Notification* notification = new Notification(context, data);
    notification->setPersistentId(persistentId);
    notification->setState(showing ? NotificationStateShowing : NotificationStateClosed);
    notification->suspendIfNeeded();

    return notification;
}
