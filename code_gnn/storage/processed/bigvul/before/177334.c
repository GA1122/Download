void ACodec::LoadedState::stateEntered() {
    ALOGV("[%s] Now Loaded", mCodec->mComponentName.c_str());

    mCodec->mPortEOS[kPortIndexInput] =
        mCodec->mPortEOS[kPortIndexOutput] = false;

    mCodec->mInputEOSResult = OK;

    mCodec->mDequeueCounter = 0;
    mCodec->mMetadataBuffersToSubmit = 0;
    mCodec->mRepeatFrameDelayUs = -1ll;
    mCodec->mInputFormat.clear();
    mCodec->mOutputFormat.clear();
    mCodec->mBaseOutputFormat.clear();

 if (mCodec->mShutdownInProgress) {
 bool keepComponentAllocated = mCodec->mKeepComponentAllocated;

        mCodec->mShutdownInProgress = false;
        mCodec->mKeepComponentAllocated = false;

        onShutdown(keepComponentAllocated);
 }
    mCodec->mExplicitShutdown = false;

    mCodec->processDeferredMessages();
}
