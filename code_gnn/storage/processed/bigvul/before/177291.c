void ACodec::processDeferredMessages() {
 List<sp<AMessage> > queue = mDeferredQueue;
    mDeferredQueue.clear();

 List<sp<AMessage> >::iterator it = queue.begin();
 while (it != queue.end()) {
        onMessageReceived(*it++);
 }
}
