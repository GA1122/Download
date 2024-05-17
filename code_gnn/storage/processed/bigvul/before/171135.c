status_t MediaPlayerService::Client::attachAuxEffect(int effectId)
{
    ALOGV("[%d] attachAuxEffect(%d)", mConnId, effectId);
 Mutex::Autolock l(mLock);
 if (mAudioOutput != 0) return mAudioOutput->attachAuxEffect(effectId);
 return NO_ERROR;
}
