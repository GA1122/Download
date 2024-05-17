void NuPlayer::GenericSource::readBuffer(
        media_track_type trackType, int64_t seekTimeUs, int64_t *actualTimeUs, bool formatChange) {
 if (mStopRead) {
 return;
 }
 Track *track;
 size_t maxBuffers = 1;
 switch (trackType) {
 case MEDIA_TRACK_TYPE_VIDEO:
            track = &mVideoTrack;
 if (mIsWidevine) {
                maxBuffers = 2;
 } else {
                maxBuffers = 4;
 }
 break;
 case MEDIA_TRACK_TYPE_AUDIO:
            track = &mAudioTrack;
 if (mIsWidevine) {
                maxBuffers = 8;
 } else {
                maxBuffers = 64;
 }
 break;
 case MEDIA_TRACK_TYPE_SUBTITLE:
            track = &mSubtitleTrack;
 break;
 case MEDIA_TRACK_TYPE_TIMEDTEXT:
            track = &mTimedTextTrack;
 break;
 default:
            TRESPASS();
 }

 if (track->mSource == NULL) {
 return;
 }

 if (actualTimeUs) {
 *actualTimeUs = seekTimeUs;
 }

 MediaSource::ReadOptions options;

 bool seeking = false;

 if (seekTimeUs >= 0) {
        options.setSeekTo(seekTimeUs, MediaSource::ReadOptions::SEEK_PREVIOUS_SYNC);
        seeking = true;
 }

 if (mIsWidevine) {
        options.setNonBlocking();
 }

 for (size_t numBuffers = 0; numBuffers < maxBuffers; ) {
 MediaBuffer *mbuf;
 status_t err = track->mSource->read(&mbuf, &options);

        options.clearSeekTo();

 if (err == OK) {
 int64_t timeUs;
            CHECK(mbuf->meta_data()->findInt64(kKeyTime, &timeUs));
 if (trackType == MEDIA_TRACK_TYPE_AUDIO) {
                mAudioTimeUs = timeUs;
 } else if (trackType == MEDIA_TRACK_TYPE_VIDEO) {
                mVideoTimeUs = timeUs;
 }

            queueDiscontinuityIfNeeded(seeking, formatChange, trackType, track);

            sp<ABuffer> buffer = mediaBufferToABuffer(
                    mbuf, trackType, seekTimeUs, actualTimeUs);
            track->mPackets->queueAccessUnit(buffer);
            formatChange = false;
            seeking = false;
 ++numBuffers;
 } else if (err == WOULD_BLOCK) {
 break;
 } else if (err == INFO_FORMAT_CHANGED) {
#if 0
            track->mPackets->queueDiscontinuity(
 ATSParser::DISCONTINUITY_FORMATCHANGE,
                    NULL,
 false  );
#endif
 } else {
            queueDiscontinuityIfNeeded(seeking, formatChange, trackType, track);
            track->mPackets->signalEOS(err);
 break;
 }
 }
}
