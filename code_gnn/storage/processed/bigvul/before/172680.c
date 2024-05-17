status_t MediaPlayer::setAuxEffectSendLevel(float level)
{
    ALOGV("MediaPlayer::setAuxEffectSendLevel(%f)", level);
 Mutex::Autolock _l(mLock);
    mSendLevel = level;
 if (mPlayer != 0) {
 return mPlayer->setAuxEffectSendLevel(level);
 }
 return OK;
}
