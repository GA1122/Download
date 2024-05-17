status_t OggSource::read(
 MediaBuffer **out, const ReadOptions *options) {
 *out = NULL;

 int64_t seekTimeUs;
 ReadOptions::SeekMode mode;
 if (options && options->getSeekTo(&seekTimeUs, &mode)) {
 status_t err = mExtractor->mImpl->seekToTime(seekTimeUs);
 if (err != OK) {
 return err;
 }
 }

 MediaBuffer *packet;
 status_t err = mExtractor->mImpl->readNextPacket(&packet);

 if (err != OK) {
 return err;
 }

#if 0
 int64_t timeUs;
 if (packet->meta_data()->findInt64(kKeyTime, &timeUs)) {
        ALOGI("found time = %lld us", timeUs);
 } else {
        ALOGI("NO time");
 }
#endif

    packet->meta_data()->setInt32(kKeyIsSyncFrame, 1);

 *out = packet;

 return OK;
}
