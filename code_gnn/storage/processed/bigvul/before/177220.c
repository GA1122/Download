void ACodec::deferMessage(const sp<AMessage> &msg) {
    mDeferredQueue.push_back(msg);
}
