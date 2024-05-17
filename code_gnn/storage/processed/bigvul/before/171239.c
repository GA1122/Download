void SoundTriggerHwService::Module::detach() {
    ALOGV("detach()");
 if (!captureHotwordAllowed()) {
 return;
 }
 {
 AutoMutex lock(mLock);
 for (size_t i = 0; i < mModels.size(); i++) {
            sp<Model> model = mModels.valueAt(i);
            ALOGV("detach() unloading model %d", model->mHandle);
 if (model->mState == Model::STATE_ACTIVE) {
                mHwDevice->stop_recognition(mHwDevice, model->mHandle);
 }
            mHwDevice->unload_sound_model(mHwDevice, model->mHandle);
 }
        mModels.clear();
 }
 if (mClient != 0) {
 IInterface::asBinder(mClient)->unlinkToDeath(this);
 }
    sp<SoundTriggerHwService> service = mService.promote();
 if (service == 0) {
 return;
 }
    service->detachModule(this);
}
