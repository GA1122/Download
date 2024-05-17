 void WebPage::updateNotificationPermission(const BlackBerry::Platform::String& requestId, bool allowed)
{
#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
    d->notificationManager().updatePermission(requestId, allowed);
#else
    UNUSED_PARAM(requestId);
    UNUSED_PARAM(allowed);
#endif
}
