void ACodec::initiateSetInputSurface(
 const sp<PersistentSurface> &surface) {
    sp<AMessage> msg = new AMessage(kWhatSetInputSurface, this);
    msg->setObject("input-surface", surface);
    msg->post();
}
