bool OMXCodec::flushPortAsync(OMX_U32 portIndex) {
    CHECK(mState == EXECUTING || mState == RECONFIGURING
 || mState == EXECUTING_TO_IDLE);

    CODEC_LOGV("flushPortAsync(%u): we own %zu out of %zu buffers already.",
         portIndex, countBuffersWeOwn(mPortBuffers[portIndex]),
         mPortBuffers[portIndex].size());

    CHECK_EQ((int)mPortStatus[portIndex], (int)ENABLED);
    mPortStatus[portIndex] = SHUTTING_DOWN;

 if ((mQuirks & kRequiresFlushCompleteEmulation)
 && countBuffersWeOwn(mPortBuffers[portIndex])
 == mPortBuffers[portIndex].size()) {

 return false;
 }

 status_t err =
        mOMX->sendCommand(mNode, OMX_CommandFlush, portIndex);
    CHECK_EQ(err, (status_t)OK);

 return true;
}
