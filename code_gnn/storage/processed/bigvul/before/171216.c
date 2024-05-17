status_t MediaPlayerService::Client::setVolume(float leftVolume, float rightVolume)
{
    ALOGV("[%d] setVolume(%f, %f)", mConnId, leftVolume, rightVolume);

    sp<MediaPlayerBase> p = getPlayer();
 {
 Mutex::Autolock l(mLock);
 if (p != 0 && p->hardwareOutput()) {
 MediaPlayerHWInterface* hwp =
 reinterpret_cast<MediaPlayerHWInterface*>(p.get());
 return hwp->setVolume(leftVolume, rightVolume);
 } else {
 if (mAudioOutput != 0) mAudioOutput->setVolume(leftVolume, rightVolume);
 return NO_ERROR;
 }
 }

 return NO_ERROR;
}
