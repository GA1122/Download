void ACodec::UninitializedState::stateEntered() {
    ALOGV("Now uninitialized");

 if (mDeathNotifier != NULL) {
 IInterface::asBinder(mCodec->mOMX)->unlinkToDeath(mDeathNotifier);
        mDeathNotifier.clear();
 }

    mCodec->mNativeWindow.clear();
    mCodec->mNativeWindowUsageBits = 0;
    mCodec->mNode = 0;
    mCodec->mOMX.clear();
    mCodec->mQuirks = 0;
    mCodec->mFlags = 0;
    mCodec->mInputMetadataType = kMetadataBufferTypeInvalid;
    mCodec->mOutputMetadataType = kMetadataBufferTypeInvalid;
    mCodec->mComponentName.clear();
}
