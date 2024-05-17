IMediaDeathNotifier::DeathNotifier::~DeathNotifier()
{
    ALOGV("DeathNotifier::~DeathNotifier");
 Mutex::Autolock _l(sServiceLock);
    sObitRecipients.clear();
 if (sMediaPlayerService != 0) {
 IInterface::asBinder(sMediaPlayerService)->unlinkToDeath(this);
 }
}
