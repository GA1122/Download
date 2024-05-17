void MediaPlayerService::Client::disconnect()
{
    ALOGV("disconnect(%d) from pid %d", mConnId, mPid);
    sp<MediaPlayerBase> p;
 {
 Mutex::Autolock l(mLock);
        p = mPlayer;
        mClient.clear();
 }

    mPlayer.clear();

 if (p != 0) {
        p->setNotifyCallback(0, 0);
#if CALLBACK_ANTAGONIZER
        ALOGD("kill Antagonizer");
        mAntagonizer->kill();
#endif
        p->reset();
 }

    disconnectNativeWindow();

 IPCThreadState::self()->flushCommands();
}
