void NuPlayer::GenericSource::start() {
    ALOGI("start");

    mStopRead = false;
 if (mAudioTrack.mSource != NULL) {
        CHECK_EQ(mAudioTrack.mSource->start(), (status_t)OK);

        postReadBuffer(MEDIA_TRACK_TYPE_AUDIO);
 }

 if (mVideoTrack.mSource != NULL) {
        CHECK_EQ(mVideoTrack.mSource->start(), (status_t)OK);

        postReadBuffer(MEDIA_TRACK_TYPE_VIDEO);
 }

    setDrmPlaybackStatusIfNeeded(Playback::START, getLastReadPosition() / 1000);
    mStarted = true;
}
