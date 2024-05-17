MediaMetadataRetriever::DeathNotifier::~DeathNotifier()
{
 Mutex::Autolock lock(sServiceLock);
 if (sService != 0) {
        sService->asBinder()->unlinkToDeath(this);
 }
}
