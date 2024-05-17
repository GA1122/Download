void WebPage::notificationClosed(const BlackBerry::Platform::String& notificationId)
{
#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
    d->notificationManager().notificationClosed(notificationId);
#else
    UNUSED_PARAM(notificationId);
#endif
}