status_t AudioFlinger::EffectHandle::disable()
{
    ALOGV("disable %p", this);
 if (!mHasControl) {
 return INVALID_OPERATION;
 }
 if (mEffect == 0) {
 return DEAD_OBJECT;
 }

 if (!mEnabled) {
 return NO_ERROR;
 }
    mEnabled = false;

 if (mEffect->suspended()) {
 return NO_ERROR;
 }

 status_t status = mEffect->setEnabled(false);

    sp<ThreadBase> thread = mEffect->thread().promote();
 if (thread != 0) {
        thread->checkSuspendOnEffectEnabled(mEffect, false, mEffect->sessionId());
 if (thread->type() == ThreadBase::OFFLOAD) {
 PlaybackThread *t = (PlaybackThread *)thread.get();
 Mutex::Autolock _l(t->mLock);
            t->broadcast_l();
 }
 }

 return status;
}
