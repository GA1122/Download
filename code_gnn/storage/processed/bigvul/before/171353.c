void OMXCodec::onStateChange(OMX_STATETYPE newState) {
    CODEC_LOGV("onStateChange %d", newState);

 switch (newState) {
 case OMX_StateIdle:
 {
            CODEC_LOGV("Now Idle.");
 if (mState == LOADED_TO_IDLE) {
 status_t err = mOMX->sendCommand(
                        mNode, OMX_CommandStateSet, OMX_StateExecuting);

                CHECK_EQ(err, (status_t)OK);

                setState(IDLE_TO_EXECUTING);
 } else {
                CHECK_EQ((int)mState, (int)EXECUTING_TO_IDLE);

 if (countBuffersWeOwn(mPortBuffers[kPortIndexInput]) !=
                    mPortBuffers[kPortIndexInput].size()) {
                    ALOGE("Codec did not return all input buffers "
 "(received %zu / %zu)",
                            countBuffersWeOwn(mPortBuffers[kPortIndexInput]),
                            mPortBuffers[kPortIndexInput].size());
                    TRESPASS();
 }

 if (countBuffersWeOwn(mPortBuffers[kPortIndexOutput]) !=
                    mPortBuffers[kPortIndexOutput].size()) {
                    ALOGE("Codec did not return all output buffers "
 "(received %zu / %zu)",
                            countBuffersWeOwn(mPortBuffers[kPortIndexOutput]),
                            mPortBuffers[kPortIndexOutput].size());
                    TRESPASS();
 }

 status_t err = mOMX->sendCommand(
                        mNode, OMX_CommandStateSet, OMX_StateLoaded);

                CHECK_EQ(err, (status_t)OK);

                err = freeBuffersOnPort(kPortIndexInput);
                CHECK_EQ(err, (status_t)OK);

                err = freeBuffersOnPort(kPortIndexOutput);
                CHECK_EQ(err, (status_t)OK);

                mPortStatus[kPortIndexInput] = ENABLED;
                mPortStatus[kPortIndexOutput] = ENABLED;

 if ((mFlags & kEnableGrallocUsageProtected) &&
                        mNativeWindow != NULL) {
                    pushBlankBuffersToNativeWindow(mNativeWindow.get());
 }

                setState(IDLE_TO_LOADED);
 }
 break;
 }

 case OMX_StateExecuting:
 {
            CHECK_EQ((int)mState, (int)IDLE_TO_EXECUTING);

            CODEC_LOGV("Now Executing.");

            mOutputPortSettingsChangedPending = false;

            setState(EXECUTING);

 break;
 }

 case OMX_StateLoaded:
 {
            CHECK_EQ((int)mState, (int)IDLE_TO_LOADED);

            CODEC_LOGV("Now Loaded.");

            setState(LOADED);
 break;
 }

 case OMX_StateInvalid:
 {
            setState(ERROR);
 break;
 }

 default:
 {
            CHECK(!"should not be here.");
 break;
 }
 }
}
