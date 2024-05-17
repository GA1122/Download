void WebPage::notificationError(const BlackBerry::Platform::String& notificationId)
{
#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
    d->notificationManager().notificationError(notificationId);
#else
    UNUSED_PARAM(notificationId);
#endif
}
