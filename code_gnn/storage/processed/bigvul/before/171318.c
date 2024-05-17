void OMXCodec::disablePortAsync(OMX_U32 portIndex) {
    CHECK(mState == EXECUTING || mState == RECONFIGURING);

    CHECK_EQ((int)mPortStatus[portIndex], (int)ENABLED);
    mPortStatus[portIndex] = DISABLING;

    CODEC_LOGV("sending OMX_CommandPortDisable(%u)", portIndex);
 status_t err =
        mOMX->sendCommand(mNode, OMX_CommandPortDisable, portIndex);
    CHECK_EQ(err, (status_t)OK);

    freeBuffersOnPort(portIndex, true);
}
