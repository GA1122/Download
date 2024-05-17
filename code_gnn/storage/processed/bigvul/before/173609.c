void NuPlayer::GenericSource::sendTextData(
 uint32_t what,
        media_track_type type,
 int32_t curGen,
        sp<AnotherPacketSource> packets,
        sp<AMessage> msg) {
 int32_t msgGeneration;
    CHECK(msg->findInt32("generation", &msgGeneration));
 if (msgGeneration != curGen) {
 return;
 }

 int64_t subTimeUs;
 if (packets->nextBufferTime(&subTimeUs) != OK) {
 return;
 }

 int64_t nextSubTimeUs;
    readBuffer(type, -1, &nextSubTimeUs);

    sp<ABuffer> buffer;
 status_t dequeueStatus = packets->dequeueAccessUnit(&buffer);
 if (dequeueStatus == OK) {
        sp<AMessage> notify = dupNotify();
        notify->setInt32("what", what);
        notify->setBuffer("buffer", buffer);
        notify->post();

 const int64_t delayUs = nextSubTimeUs - subTimeUs;
        msg->post(delayUs < 0 ? 0 : delayUs);
 }
}
