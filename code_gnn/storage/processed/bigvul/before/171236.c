SoundTriggerHwService::SoundTriggerHwService()
 : BnSoundTriggerHwService(),
      mNextUniqueId(1),
      mMemoryDealer(new MemoryDealer(1024 * 1024, "SoundTriggerHwService")),
      mCaptureState(false)
{
}
