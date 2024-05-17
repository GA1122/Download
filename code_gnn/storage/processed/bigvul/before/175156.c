ssize_t NuPlayer::GenericSource::getSelectedTrack(media_track_type type) const {
    sp<AMessage> msg = new AMessage(kWhatGetSelectedTrack, id());
    msg->setInt32("type", type);

    sp<AMessage> response;
 int32_t index;
 status_t err = msg->postAndAwaitResponse(&response);
 if (err == OK && response != NULL) {
        CHECK(response->findInt32("index", &index));
 return index;
 } else {
 return -1;
 }
}
