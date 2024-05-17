status_t MediaPlayerService::AudioOutput::attachAuxEffect(int effectId)
{
    ALOGV("attachAuxEffect(%d)", effectId);
 Mutex::Autolock lock(mLock);
    mAuxEffectId = effectId;
 if (mTrack != 0) {
 return mTrack->attachAuxEffect(effectId);
 }
 return NO_ERROR;
}
