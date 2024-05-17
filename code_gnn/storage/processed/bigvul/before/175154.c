void NuPlayer::GenericSource::fetchTextData(
 uint32_t sendWhat,
        media_track_type type,
 int32_t curGen,
        sp<AnotherPacketSource> packets,
        sp<AMessage> msg) {
 int32_t msgGeneration;
    CHECK(msg->findInt32("generation", &msgGeneration));
 if (msgGeneration != curGen) {
 return;
 }

 int32_t avail;
 if (packets->hasBufferAvailable(&avail)) {
 return;
 }

 int64_t timeUs;
    CHECK(msg->findInt64("timeUs", &timeUs));

 int64_t subTimeUs;
    readBuffer(type, timeUs, &subTimeUs);

 int64_t delayUs = subTimeUs - timeUs;
 if (msg->what() == kWhatFetchSubtitleData) {
 const int64_t oneSecUs = 1000000ll;
        delayUs -= oneSecUs;
 }
    sp<AMessage> msg2 = new AMessage(sendWhat, id());
    msg2->setInt32("generation", msgGeneration);
    msg2->post(delayUs < 0 ? 0 : delayUs);
}
