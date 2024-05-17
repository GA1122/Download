void NuPlayer::GenericSource::onSeek(sp<AMessage> msg) {
 int64_t seekTimeUs;
    CHECK(msg->findInt64("seekTimeUs", &seekTimeUs));

    sp<AMessage> response = new AMessage;
 status_t err = doSeek(seekTimeUs);
    response->setInt32("err", err);

 uint32_t replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}
