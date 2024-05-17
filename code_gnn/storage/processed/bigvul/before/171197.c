status_t MediaPlayerService::Client::setAuxEffectSendLevel(float level)
{
    ALOGV("[%d] setAuxEffectSendLevel(%f)", mConnId, level);
 Mutex::Autolock l(mLock);
 if (mAudioOutput != 0) return mAudioOutput->setAuxEffectSendLevel(level);
 return NO_ERROR;
}
