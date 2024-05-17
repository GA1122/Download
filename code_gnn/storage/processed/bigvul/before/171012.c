void AudioFlinger::EffectChain::process_l()
{
    sp<ThreadBase> thread = mThread.promote();
 if (thread == 0) {
        ALOGW("process_l(): cannot promote mixer thread");
 return;
 }
 bool isGlobalSession = (mSessionId == AUDIO_SESSION_OUTPUT_MIX) ||
 (mSessionId == AUDIO_SESSION_OUTPUT_STAGE);
 bool doProcess = (thread->type() != ThreadBase::OFFLOAD);
 if (!isGlobalSession) {
 bool tracksOnSession = (trackCnt() != 0);

 if (!tracksOnSession && mTailBufferCount == 0) {
            doProcess = false;
 }

 if (activeTrackCnt() == 0) {
 if (tracksOnSession || mTailBufferCount > 0) {
                clearInputBuffer_l(thread);
 if (mTailBufferCount > 0) {
                    mTailBufferCount--;
 }
 }
 }
 }

 size_t size = mEffects.size();
 if (doProcess) {
 for (size_t i = 0; i < size; i++) {
            mEffects[i]->process();
 }
 }
 for (size_t i = 0; i < size; i++) {
        mEffects[i]->updateState();
 }
}
