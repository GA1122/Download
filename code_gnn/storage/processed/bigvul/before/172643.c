IMediaDeathNotifier::addObitRecipient(const wp<IMediaDeathNotifier>& recipient)
{
    ALOGV("addObitRecipient");
 Mutex::Autolock _l(sServiceLock);
    sObitRecipients.add(recipient);
}
