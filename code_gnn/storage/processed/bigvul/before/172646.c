IMediaDeathNotifier::removeObitRecipient(const wp<IMediaDeathNotifier>& recipient)
{
    ALOGV("removeObitRecipient");
 Mutex::Autolock _l(sServiceLock);
    sObitRecipients.remove(recipient);
}
