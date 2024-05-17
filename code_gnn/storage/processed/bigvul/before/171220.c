status_t MediaPlayerService::AudioOutput::start()
{
    ALOGV("start");
 Mutex::Autolock lock(mLock);
 if (mCallbackData != NULL) {
        mCallbackData->endTrackSwitch();
 }
 if (mTrack != 0) {
        mTrack->setVolume(mLeftVolume, mRightVolume);
        mTrack->setAuxEffectSendLevel(mSendLevel);
 return mTrack->start();
 }
 return NO_INIT;
}
