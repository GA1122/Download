void NuPlayer::GenericSource::onGetFormatMeta(sp<AMessage> msg) const {
 int32_t audio;
    CHECK(msg->findInt32("audio", &audio));

    sp<AMessage> response = new AMessage;
    sp<MetaData> format = doGetFormatMeta(audio);
    response->setPointer("format", format.get());

 uint32_t replyID;
    CHECK(msg->senderAwaitsResponse(&replyID));
    response->postReply(replyID);
}
