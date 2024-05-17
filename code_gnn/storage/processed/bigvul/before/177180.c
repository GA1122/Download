ACodec::ACodec()
 : mQuirks(0),
      mNode(0),
      mNativeWindowUsageBits(0),
      mSentFormat(false),
      mIsVideo(false),
      mIsEncoder(false),
      mFatalError(false),
      mShutdownInProgress(false),
      mExplicitShutdown(false),
      mEncoderDelay(0),
      mEncoderPadding(0),
      mRotationDegrees(0),
      mChannelMaskPresent(false),
      mChannelMask(0),
      mDequeueCounter(0),
      mInputMetadataType(kMetadataBufferTypeInvalid),
      mOutputMetadataType(kMetadataBufferTypeInvalid),
      mLegacyAdaptiveExperiment(false),
      mMetadataBuffersToSubmit(0),
      mRepeatFrameDelayUs(-1ll),
      mMaxPtsGapUs(-1ll),
      mMaxFps(-1),
      mTimePerFrameUs(-1ll),
      mTimePerCaptureUs(-1ll),
      mCreateInputBuffersSuspended(false),
      mTunneled(false) {
    mUninitializedState = new UninitializedState(this);
    mLoadedState = new LoadedState(this);
    mLoadedToIdleState = new LoadedToIdleState(this);
    mIdleToExecutingState = new IdleToExecutingState(this);
    mExecutingState = new ExecutingState(this);

    mOutputPortSettingsChangedState =
 new OutputPortSettingsChangedState(this);

    mExecutingToIdleState = new ExecutingToIdleState(this);
    mIdleToLoadedState = new IdleToLoadedState(this);
    mFlushingState = new FlushingState(this);

    mPortEOS[kPortIndexInput] = mPortEOS[kPortIndexOutput] = false;
    mInputEOSResult = OK;

    changeState(mUninitializedState);
}
