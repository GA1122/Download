status_t AudioFlinger::EffectModule::init()
{
 Mutex::Autolock _l(mLock);
 if (mEffectInterface == NULL) {
 return NO_INIT;
 }
 status_t cmdStatus;
 uint32_t size = sizeof(status_t);
 status_t status = (*mEffectInterface)->command(mEffectInterface,
                                                   EFFECT_CMD_INIT,
 0,
                                                   NULL,
 &size,
 &cmdStatus);
 if (status == 0) {
        status = cmdStatus;
 }
 return status;
}
