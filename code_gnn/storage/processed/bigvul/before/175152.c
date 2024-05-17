status_t NuPlayer::GenericSource::doSeek(int64_t seekTimeUs) {
 if (mStopRead) {
 return INVALID_OPERATION;
 }
 if (mVideoTrack.mSource != NULL) {
 int64_t actualTimeUs;
        readBuffer(MEDIA_TRACK_TYPE_VIDEO, seekTimeUs, &actualTimeUs);

        seekTimeUs = actualTimeUs;
 }

 if (mAudioTrack.mSource != NULL) {
        readBuffer(MEDIA_TRACK_TYPE_AUDIO, seekTimeUs);
 }

    setDrmPlaybackStatusIfNeeded(Playback::START, seekTimeUs / 1000);
 if (!mStarted) {
        setDrmPlaybackStatusIfNeeded(Playback::PAUSE, 0);
 }
 return OK;
}
