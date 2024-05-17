void NuPlayer::GenericSource::onGetSelectedTrack(sp<AMessage> msg) const {
 int32_t tmpType;
    CHECK(msg->findInt32("type", &tmpType));
    media_track_type type = (media_track_type)tmpType;

    sp<AMessage> response = new AMessage;
 ssize_t index = doGetSelectedTrack(type);
    response->setInt32("index", index);

 uint32_t replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}
