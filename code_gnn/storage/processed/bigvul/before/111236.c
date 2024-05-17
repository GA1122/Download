void WebPage::notificationClicked(const BlackBerry::Platform::String& notificationId)
{
#if ENABLE(NOTIFICATIONS) || ENABLE(LEGACY_NOTIFICATIONS)
    d->notificationManager().notificationClicked(notificationId);
#else
    UNUSED_PARAM(notificationId);
#endif
}
