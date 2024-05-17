void OMXCodec::onCmdComplete(OMX_COMMANDTYPE cmd, OMX_U32 data) {
 switch (cmd) {
 case OMX_CommandStateSet:
 {
            onStateChange((OMX_STATETYPE)data);
 break;
 }

 case OMX_CommandPortDisable:
 {
            OMX_U32 portIndex = data;
            CODEC_LOGV("PORT_DISABLED(%u)", portIndex);

            CHECK(mState == EXECUTING || mState == RECONFIGURING);
            CHECK_EQ((int)mPortStatus[portIndex], (int)DISABLING);
            CHECK_EQ(mPortBuffers[portIndex].size(), 0u);

            mPortStatus[portIndex] = DISABLED;

 if (mState == RECONFIGURING) {
                CHECK_EQ(portIndex, (OMX_U32)kPortIndexOutput);

                sp<MetaData> oldOutputFormat = mOutputFormat;
                initOutputFormat(mSource->getFormat());

 bool formatChanged = formatHasNotablyChanged(oldOutputFormat, mOutputFormat);
 if (!mOutputPortSettingsHaveChanged) {
                    mOutputPortSettingsHaveChanged = formatChanged;
 }

 status_t err = enablePortAsync(portIndex);
 if (err != OK) {
                    CODEC_LOGE("enablePortAsync(%u) failed (err = %d)", portIndex, err);
                    setState(ERROR);
 } else {
                    err = allocateBuffersOnPort(portIndex);
 if (err != OK) {
                        CODEC_LOGE("allocateBuffersOnPort (%s) failed "
 "(err = %d)",
                                   portIndex == kPortIndexInput
 ? "input" : "output",
                                   err);

                        setState(ERROR);
 }
 }
 }
 break;
 }

 case OMX_CommandPortEnable:
 {
            OMX_U32 portIndex = data;
            CODEC_LOGV("PORT_ENABLED(%u)", portIndex);

            CHECK(mState == EXECUTING || mState == RECONFIGURING);
            CHECK_EQ((int)mPortStatus[portIndex], (int)ENABLING);

            mPortStatus[portIndex] = ENABLED;

 if (mState == RECONFIGURING) {
                CHECK_EQ(portIndex, (OMX_U32)kPortIndexOutput);

                setState(EXECUTING);

                fillOutputBuffers();
 }
 break;
 }

 case OMX_CommandFlush:
 {
            OMX_U32 portIndex = data;

            CODEC_LOGV("FLUSH_DONE(%u)", portIndex);

            CHECK_EQ((int)mPortStatus[portIndex], (int)SHUTTING_DOWN);
            mPortStatus[portIndex] = ENABLED;

            CHECK_EQ(countBuffersWeOwn(mPortBuffers[portIndex]),
                     mPortBuffers[portIndex].size());

 if (mSkipCutBuffer != NULL && mPortStatus[kPortIndexOutput] == ENABLED) {
                mSkipCutBuffer->clear();
 }

 if (mState == RECONFIGURING) {
                CHECK_EQ(portIndex, (OMX_U32)kPortIndexOutput);

                disablePortAsync(portIndex);
 } else if (mState == EXECUTING_TO_IDLE) {
 if (mPortStatus[kPortIndexInput] == ENABLED
 && mPortStatus[kPortIndexOutput] == ENABLED) {
                    CODEC_LOGV("Finished flushing both ports, now completing "
 "transition from EXECUTING to IDLE.");

                    mPortStatus[kPortIndexInput] = SHUTTING_DOWN;
                    mPortStatus[kPortIndexOutput] = SHUTTING_DOWN;

 status_t err =
                        mOMX->sendCommand(mNode, OMX_CommandStateSet, OMX_StateIdle);
                    CHECK_EQ(err, (status_t)OK);
 }
 } else {

 if (mPortStatus[kPortIndexInput] == ENABLED
 && mPortStatus[kPortIndexOutput] == ENABLED) {
                    CODEC_LOGV("Finished flushing both ports, now continuing from"
 " seek-time.");

                    mPaused = false;

                    drainInputBuffers();
                    fillOutputBuffers();
 }

 if (mOutputPortSettingsChangedPending) {
                    CODEC_LOGV(
 "Honoring deferred output port settings change.");

                    mOutputPortSettingsChangedPending = false;
                    onPortSettingsChanged(kPortIndexOutput);
 }
 }

 break;
 }

 default:
 {
            CODEC_LOGV("CMD_COMPLETE(%d, %u)", cmd, data);
 break;
 }
 }
}
