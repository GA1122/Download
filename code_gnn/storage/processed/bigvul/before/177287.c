void ACodec::onSignalEndOfInputStream() {
    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", CodecBase::kWhatSignaledInputEOS);

 status_t err = mOMX->signalEndOfInputStream(mNode);
 if (err != OK) {
        notify->setInt32("err", err);
 }
    notify->post();
}
