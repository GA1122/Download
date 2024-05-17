status_t AudioFlinger::EffectModule::setMode(audio_mode_t mode)
{
 Mutex::Autolock _l(mLock);
 if (mStatus != NO_ERROR) {
 return mStatus;
 }
 status_t status = NO_ERROR;
 if ((mDescriptor.flags & EFFECT_FLAG_AUDIO_MODE_MASK) == EFFECT_FLAG_AUDIO_MODE_IND) {
 status_t cmdStatus;
 uint32_t size = sizeof(status_t);
        status = (*mEffectInterface)->command(mEffectInterface,
                                              EFFECT_CMD_SET_AUDIO_MODE,
 sizeof(audio_mode_t),
 &mode,
 &size,
 &cmdStatus);
 if (status == NO_ERROR) {
            status = cmdStatus;
 }
 }
 return status;
}
