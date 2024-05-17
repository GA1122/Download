void AudioFlinger::EffectModule::reset_l()
{
 if (mStatus != NO_ERROR || mEffectInterface == NULL) {
 return;
 }
 (*mEffectInterface)->command(mEffectInterface, EFFECT_CMD_RESET, 0, NULL, 0, NULL);
}
