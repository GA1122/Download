void WebPage::notificationShown(const BlackBerry::Platform::String& notificationId)
{
#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
    d->notificationManager().notificationShown(notificationId);
#else
    UNUSED_PARAM(notificationId);
#endif
}
