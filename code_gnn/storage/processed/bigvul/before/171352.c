void OMXCodec::onPortSettingsChanged(OMX_U32 portIndex) {
    CODEC_LOGV("PORT_SETTINGS_CHANGED(%u)", portIndex);

    CHECK(mState == EXECUTING || mState == EXECUTING_TO_IDLE);
    CHECK_EQ(portIndex, (OMX_U32)kPortIndexOutput);
    CHECK(!mOutputPortSettingsChangedPending);

 if (mPortStatus[kPortIndexOutput] != ENABLED) {
        CODEC_LOGV("Deferring output port settings change.");
        mOutputPortSettingsChangedPending = true;
 return;
 }

    setState(RECONFIGURING);

 if (mQuirks & kNeedsFlushBeforeDisable) {
 if (!flushPortAsync(portIndex)) {
            onCmdComplete(OMX_CommandFlush, portIndex);
 }
 } else {
        disablePortAsync(portIndex);
 }
}
