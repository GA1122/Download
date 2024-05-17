OMX_ERRORTYPE SimpleSoftOMXComponent::fillThisBuffer(
        OMX_BUFFERHEADERTYPE *buffer) {
    sp<AMessage> msg = new AMessage(kWhatFillThisBuffer, mHandler);
    msg->setPointer("header", buffer);
    msg->post();

 return OMX_ErrorNone;
}
