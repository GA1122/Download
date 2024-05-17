void ACodec::ExecutingToIdleState::onInputBufferFilled(
 const sp<AMessage> &msg) {
 BaseState::onInputBufferFilled(msg);

    changeStateIfWeOwnAllBuffers();
}
