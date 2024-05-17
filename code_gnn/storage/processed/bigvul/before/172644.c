IMediaDeathNotifier::DeathNotifier::binderDied(const wp<IBinder>& who __unused) {
    ALOGW("media server died");

 SortedVector< wp<IMediaDeathNotifier> > list;
 {
 Mutex::Autolock _l(sServiceLock);
        sMediaPlayerService.clear();
 list = sObitRecipients;
 }

 size_t count = list.size();
 for (size_t iter = 0; iter < count; ++iter) {
        sp<IMediaDeathNotifier> notifier = list[iter].promote();
 if (notifier != 0) {
            notifier->died();
 }
 }
}
