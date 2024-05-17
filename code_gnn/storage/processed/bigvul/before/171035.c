status_t AudioFlinger::EffectModule::start_l()
{
 if (mEffectInterface == NULL) {
 return NO_INIT;
 }
 if (mStatus != NO_ERROR) {
 return mStatus;
 }
 status_t cmdStatus;
 uint32_t size = sizeof(status_t);
 status_t status = (*mEffectInterface)->command(mEffectInterface,
                                                   EFFECT_CMD_ENABLE,
 0,
                                                   NULL,
 &size,
 &cmdStatus);
 if (status == 0) {
        status = cmdStatus;
 }
 if (status == 0) {
        addEffectToHal_l();
        sp<EffectChain> chain = mChain.promote();
 if (chain != 0) {
            chain->forceVolume();
 }
 }
 return status;
}
