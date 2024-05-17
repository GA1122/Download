void ACodec::LoadedToIdleState::stateEntered() {
    ALOGV("[%s] Now Loaded->Idle", mCodec->mComponentName.c_str());

 status_t err;
 if ((err = allocateBuffers()) != OK) {
        ALOGE("Failed to allocate buffers after transitioning to IDLE state "
 "(error 0x%08x)",
             err);

        mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));

        mCodec->changeState(mCodec->mLoadedState);
 }
}
