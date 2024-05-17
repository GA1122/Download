void ATSParser::SyncEvent::init(off64_t offset, const sp<MediaSource> &source,
 int64_t timeUs) {
    mInit = true;
    mOffset = offset;
    mMediaSource = source;
    mTimeUs = timeUs;
}
