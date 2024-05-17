status_t AudioFlinger::EffectModule::stop_l()
{
 if (mEffectInterface == NULL) {
 return NO_INIT;
 }
 if (mStatus != NO_ERROR) {
 return mStatus;
 }
 status_t cmdStatus = NO_ERROR;
 uint32_t size = sizeof(status_t);
 status_t status = (*mEffectInterface)->command(mEffectInterface,
                                                   EFFECT_CMD_DISABLE,
 0,
                                                   NULL,
 &size,
 &cmdStatus);
 if (status == NO_ERROR) {
        status = cmdStatus;
 }
 if (status == NO_ERROR) {
        status = remove_effect_from_hal_l();
 }
 return status;
}
