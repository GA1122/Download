status_t AudioFlinger::EffectModule::setDevice(audio_devices_t device)
{
 if (device == AUDIO_DEVICE_NONE) {
 return NO_ERROR;
 }

 Mutex::Autolock _l(mLock);
 if (mStatus != NO_ERROR) {
 return mStatus;
 }
 status_t status = NO_ERROR;
 if ((mDescriptor.flags & EFFECT_FLAG_DEVICE_MASK) == EFFECT_FLAG_DEVICE_IND) {
 status_t cmdStatus;
 uint32_t size = sizeof(status_t);
 uint32_t cmd = audio_is_output_devices(device) ? EFFECT_CMD_SET_DEVICE :
                            EFFECT_CMD_SET_INPUT_DEVICE;
        status = (*mEffectInterface)->command(mEffectInterface,
                                              cmd,
 sizeof(uint32_t),
 &device,
 &size,
 &cmdStatus);
 }
 return status;
}
