sp<MediaPlayerBase> MediaPlayerService::Client::setDataSource_pre(
        player_type playerType)
{
    ALOGV("player type = %d", playerType);

    sp<MediaPlayerBase> p = createPlayer(playerType);
 if (p == NULL) {
 return p;
 }

    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("media.extractor"));
    mExtractorDeathListener = new ServiceDeathNotifier(binder, p, MEDIAEXTRACTOR_PROCESS_DEATH);
    binder->linkToDeath(mExtractorDeathListener);

    binder = sm->getService(String16("media.codec"));
    mCodecDeathListener = new ServiceDeathNotifier(binder, p, MEDIACODEC_PROCESS_DEATH);
    binder->linkToDeath(mCodecDeathListener);

 if (!p->hardwareOutput()) {
 Mutex::Autolock l(mLock);
        mAudioOutput = new AudioOutput(mAudioSessionId, IPCThreadState::self()->getCallingUid(),
                mPid, mAudioAttributes);
 static_cast<MediaPlayerInterface*>(p.get())->setAudioSink(mAudioOutput);
 }

 return p;
}
