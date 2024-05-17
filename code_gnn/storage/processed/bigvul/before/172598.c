AudioFlinger::EffectModule::EffectModule(ThreadBase *thread,
 const wp<AudioFlinger::EffectChain>& chain,
 effect_descriptor_t *desc,
 int id,
 int sessionId)
 : mPinned(sessionId > AUDIO_SESSION_OUTPUT_MIX),
      mThread(thread), mChain(chain), mId(id), mSessionId(sessionId),
      mDescriptor(*desc),
      mEffectInterface(NULL),
      mStatus(NO_INIT), mState(IDLE),
      mSuspended(false),
      mAudioFlinger(thread->mAudioFlinger)
{
    ALOGV("Constructor %p", this);
 int lStatus;

    mStatus = EffectCreate(&desc->uuid, sessionId, thread->id(), &mEffectInterface);

 if (mStatus != NO_ERROR) {
 return;
 }
    lStatus = init();
 if (lStatus < 0) {
        mStatus = lStatus;
 goto Error;
 }

    ALOGV("Constructor success name %s, Interface %p", mDescriptor.name, mEffectInterface);
 return;
Error:
 EffectRelease(mEffectInterface);
    mEffectInterface = NULL;
    ALOGV("Constructor Error %d", mStatus);
}
