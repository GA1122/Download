status_t NuPlayer::GenericSource::selectTrack(size_t trackIndex, bool select) {
    ALOGV("%s track: %zu", select ? "select" : "deselect", trackIndex);
    sp<AMessage> msg = new AMessage(kWhatSelectTrack, id());
    msg->setInt32("trackIndex", trackIndex);
    msg->setInt32("select", select);

    sp<AMessage> response;
 status_t err = msg->postAndAwaitResponse(&response);
 if (err == OK && response != NULL) {
        CHECK(response->findInt32("err", &err));
 }

 return err;
}
