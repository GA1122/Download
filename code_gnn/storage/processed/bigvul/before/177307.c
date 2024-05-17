status_t ACodec::setSurface(const sp<Surface> &surface) {
    sp<AMessage> msg = new AMessage(kWhatSetSurface, this);
    msg->setObject("surface", surface);

    sp<AMessage> response;
 status_t err = msg->postAndAwaitResponse(&response);

 if (err == OK) {
 (void)response->findInt32("err", &err);
 }
 return err;
}
