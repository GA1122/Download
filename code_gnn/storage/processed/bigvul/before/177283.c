void ACodec::FlushingState::onOutputBufferDrained(const sp<AMessage> &msg) {
 BaseState::onOutputBufferDrained(msg);

    changeStateIfWeOwnAllBuffers();
}
