bool ACodec::OutputPortSettingsChangedState::onOMXEvent(
        OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2) {
 switch (event) {
 case OMX_EventCmdComplete:
 {
 if (data1 == (OMX_U32)OMX_CommandPortDisable) {
 if (data2 != (OMX_U32)kPortIndexOutput) {
                    ALOGW("ignoring EventCmdComplete CommandPortDisable for port %u", data2);
 return false;
 }

                ALOGV("[%s] Output port now disabled.", mCodec->mComponentName.c_str());

 status_t err = OK;
 if (!mCodec->mBuffers[kPortIndexOutput].isEmpty()) {
                    ALOGE("disabled port should be empty, but has %zu buffers",
                            mCodec->mBuffers[kPortIndexOutput].size());
                    err = FAILED_TRANSACTION;
 } else {
                    mCodec->mDealer[kPortIndexOutput].clear();
 }

 if (err == OK) {
                    err = mCodec->mOMX->sendCommand(
                            mCodec->mNode, OMX_CommandPortEnable, kPortIndexOutput);
 }

 if (err == OK) {
                    err = mCodec->allocateBuffersOnPort(kPortIndexOutput);
                    ALOGE_IF(err != OK, "Failed to allocate output port buffers after port "
 "reconfiguration: (%d)", err);
 }

 if (err != OK) {
                    mCodec->signalError(OMX_ErrorUndefined, makeNoSideEffectStatus(err));

                    mCodec->mShutdownInProgress = true;
                    mCodec->mKeepComponentAllocated = false;
                    mCodec->changeState(mCodec->mLoadedState);
 }

 return true;
 } else if (data1 == (OMX_U32)OMX_CommandPortEnable) {
 if (data2 != (OMX_U32)kPortIndexOutput) {
                    ALOGW("ignoring EventCmdComplete OMX_CommandPortEnable for port %u", data2);
 return false;
 }

                mCodec->mSentFormat = false;

 if (mCodec->mTunneled) {
                    sp<AMessage> dummy = new AMessage(kWhatOutputBufferDrained, mCodec);
                    mCodec->sendFormatChange(dummy);
 }

                ALOGV("[%s] Output port now reenabled.", mCodec->mComponentName.c_str());

 if (mCodec->mExecutingState->active()) {
                    mCodec->mExecutingState->submitOutputBuffers();
 }

                mCodec->changeState(mCodec->mExecutingState);

 return true;
 }

 return false;
 }

 default:
 return false;
 }
}
