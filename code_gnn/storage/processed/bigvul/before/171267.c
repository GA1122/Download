status_t SoundTriggerHwService::Module::unloadSoundModel_l(sound_model_handle_t handle)
{
 ssize_t index = mModels.indexOfKey(handle);
 if (index < 0) {
 return BAD_VALUE;
 }
    sp<Model> model = mModels.valueAt(index);
    mModels.removeItem(handle);
 if (model->mState == Model::STATE_ACTIVE) {
        mHwDevice->stop_recognition(mHwDevice, model->mHandle);
        model->mState = Model::STATE_IDLE;
 }
 AudioSystem::releaseSoundTriggerSession(model->mCaptureSession);
 return mHwDevice->unload_sound_model(mHwDevice, handle);
}
