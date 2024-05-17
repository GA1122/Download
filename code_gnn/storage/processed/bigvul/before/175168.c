void NuPlayer::GenericSource::onSelectTrack(sp<AMessage> msg) {
 int32_t trackIndex, select;
    CHECK(msg->findInt32("trackIndex", &trackIndex));
    CHECK(msg->findInt32("select", &select));

    sp<AMessage> response = new AMessage;
 status_t err = doSelectTrack(trackIndex, select);
    response->setInt32("err", err);

 uint32_t replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}
