status_t MediaPlayerService::AudioOutput::setAuxEffectSendLevel(float level)
{
    ALOGV("setAuxEffectSendLevel(%f)", level);
 Mutex::Autolock lock(mLock);
    mSendLevel = level;
 if (mTrack != 0) {
 return mTrack->setAuxEffectSendLevel(level);
 }
 return NO_ERROR;
}
