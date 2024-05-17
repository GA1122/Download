void ACodec::ExecutingToIdleState::changeStateIfWeOwnAllBuffers() {
 if (mComponentNowIdle && mCodec->allYourBuffersAreBelongToUs()) {
 status_t err = mCodec->mOMX->sendCommand(
                mCodec->mNode, OMX_CommandStateSet, OMX_StateLoaded);
 if (err == OK) {
            err = mCodec->freeBuffersOnPort(kPortIndexInput);
 status_t err2 = mCodec->freeBuffersOnPort(kPortIndexOutput);
 if (err == OK) {
                err = err2;
 }
 }

 if ((mCodec->mFlags & kFlagPushBlankBuffersToNativeWindowOnShutdown)
 && mCodec->mNativeWindow != NULL) {
            pushBlankBuffersToNativeWindow(mCodec->mNativeWindow.get());
 }

 if (err != OK) {
            mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return;
 }

        mCodec->changeState(mCodec->mIdleToLoadedState);
 }
}
