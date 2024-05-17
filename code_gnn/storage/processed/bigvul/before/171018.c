status_t AudioFlinger::EffectModule::setAudioSource(audio_source_t source)
{
 Mutex::Autolock _l(mLock);
 if (mStatus != NO_ERROR) {
 return mStatus;
 }
 status_t status = NO_ERROR;
 if ((mDescriptor.flags & EFFECT_FLAG_AUDIO_SOURCE_MASK) == EFFECT_FLAG_AUDIO_SOURCE_IND) {
 uint32_t size = 0;
        status = (*mEffectInterface)->command(mEffectInterface,
                                              EFFECT_CMD_SET_AUDIO_SOURCE,
 sizeof(audio_source_t),
 &source,
 &size,
                                              NULL);
 }
 return status;
}
