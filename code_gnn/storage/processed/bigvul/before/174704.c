void MediaMetadataRetriever::DeathNotifier::binderDied(const wp<IBinder>& who __unused) {
 Mutex::Autolock lock(MediaMetadataRetriever::sServiceLock);
 MediaMetadataRetriever::sService.clear();
    ALOGW("MediaMetadataRetriever server died!");
}
