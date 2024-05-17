void ACodec::ExecutingToIdleState::onOutputBufferDrained(
 const sp<AMessage> &msg) {
 BaseState::onOutputBufferDrained(msg);

    changeStateIfWeOwnAllBuffers();
}
