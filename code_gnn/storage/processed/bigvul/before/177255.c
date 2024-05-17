void ACodec::FlushingState::onInputBufferFilled(const sp<AMessage> &msg) {
 BaseState::onInputBufferFilled(msg);

    changeStateIfWeOwnAllBuffers();
}
