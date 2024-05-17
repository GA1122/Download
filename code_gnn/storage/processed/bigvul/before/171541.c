OMX_ERRORTYPE SimpleSoftOMXComponent::sendCommand(
        OMX_COMMANDTYPE cmd, OMX_U32 param, OMX_PTR data) {
    CHECK(data == NULL);

    sp<AMessage> msg = new AMessage(kWhatSendCommand, mHandler);
    msg->setInt32("cmd", cmd);
    msg->setInt32("param", param);
    msg->post();

 return OMX_ErrorNone;
}
