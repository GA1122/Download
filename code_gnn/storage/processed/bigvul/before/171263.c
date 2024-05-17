status_t SoundTriggerHwService::Module::stopRecognition(sound_model_handle_t handle)
{
    ALOGV("stopRecognition() model handle %d", handle);
 if (!captureHotwordAllowed()) {
 return PERMISSION_DENIED;
 }

 AutoMutex lock(mLock);
    sp<Model> model = getModel(handle);
 if (model == 0) {
 return BAD_VALUE;
 }

 if (model->mState != Model::STATE_ACTIVE) {
 return INVALID_OPERATION;
 }
    mHwDevice->stop_recognition(mHwDevice, handle);
    model->mState = Model::STATE_IDLE;
 return NO_ERROR;
}
