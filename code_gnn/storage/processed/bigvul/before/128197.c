size_t Notification::maxActions()
{
    if (!notificationManager())
        return 2;

    return notificationManager()->maxActions();
}
