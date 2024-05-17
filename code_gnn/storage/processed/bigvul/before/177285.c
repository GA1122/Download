void ACodec::UninitializedState::onSetup(
 const sp<AMessage> &msg) {
 if (onAllocateComponent(msg)
 && mCodec->mLoadedState->onConfigureComponent(msg)) {
        mCodec->mLoadedState->onStart();
 }
}
