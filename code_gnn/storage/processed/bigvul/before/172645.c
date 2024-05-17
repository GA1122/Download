 IMediaDeathNotifier::getMediaPlayerService()
 {
     ALOGV("getMediaPlayerService");
 Mutex::Autolock _l(sServiceLock);
 if (sMediaPlayerService == 0) {
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;
 do {
            binder = sm->getService(String16("media.player"));
 if (binder != 0) {
 break;
 }
            ALOGW("Media player service not published, waiting...");
            usleep(500000);  
 } while (true);

 if (sDeathNotifier == NULL) {
            sDeathNotifier = new DeathNotifier();
 }
        binder->linkToDeath(sDeathNotifier);
        sMediaPlayerService = interface_cast<IMediaPlayerService>(binder);
 }
    ALOGE_IF(sMediaPlayerService == 0, "no media player service!?");
 return sMediaPlayerService;
}
