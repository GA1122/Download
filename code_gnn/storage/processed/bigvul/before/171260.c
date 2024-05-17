status_t SoundTriggerHwService::setCaptureState(bool active)
{
    ALOGV("setCaptureState %d", active);
 AutoMutex lock(mServiceLock);
    mCaptureState = active;
 for (size_t i = 0; i < mModules.size(); i++) {
        mModules.valueAt(i)->setCaptureState_l(active);
 }
 return NO_ERROR;
}
