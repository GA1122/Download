void ACodec::LoadedState::onStart() {
    ALOGV("onStart");

 status_t err = mCodec->mOMX->sendCommand(mCodec->mNode, OMX_CommandStateSet, OMX_StateIdle);
 if (err != OK) {
        mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));
 } else {
        mCodec->changeState(mCodec->mLoadedToIdleState);
 }
}
