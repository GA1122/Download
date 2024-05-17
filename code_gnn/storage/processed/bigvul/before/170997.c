status_t AudioFlinger::EffectHandle::enable()
{
    ALOGV("enable %p", this);
 if (!mHasControl) {
 return INVALID_OPERATION;
 }
 if (mEffect == 0) {
 return DEAD_OBJECT;
 }

 if (mEnabled) {
 return NO_ERROR;
 }

    mEnabled = true;

    sp<ThreadBase> thread = mEffect->thread().promote();
 if (thread != 0) {
        thread->checkSuspendOnEffectEnabled(mEffect, true, mEffect->sessionId());
 }

 if (mEffect->suspended()) {
 return NO_ERROR;
 }

 status_t status = mEffect->setEnabled(true);
 if (status != NO_ERROR) {
 if (thread != 0) {
            thread->checkSuspendOnEffectEnabled(mEffect, false, mEffect->sessionId());
 }
        mEnabled = false;
 } else {
 if (thread != 0) {
 if (thread->type() == ThreadBase::OFFLOAD) {
 PlaybackThread *t = (PlaybackThread *)thread.get();
 Mutex::Autolock _l(t->mLock);
                t->broadcast_l();
 }
 if (!mEffect->isOffloadable()) {
 if (thread->type() == ThreadBase::OFFLOAD) {
 PlaybackThread *t = (PlaybackThread *)thread.get();
                    t->invalidateTracks(AUDIO_STREAM_MUSIC);
 }
 if (mEffect->sessionId() == AUDIO_SESSION_OUTPUT_MIX) {
                    thread->mAudioFlinger->onNonOffloadableGlobalEffectEnable();
 }
 }
 }
 }
 return status;
}
