void NuPlayer::GenericSource::start() {
    ALOGI("start");

    mStopRead = false;
 if (mAudioTrack.mSource != NULL) {
        postReadBuffer(MEDIA_TRACK_TYPE_AUDIO);
 }

 if (mVideoTrack.mSource != NULL) {
        postReadBuffer(MEDIA_TRACK_TYPE_VIDEO);
 }

    setDrmPlaybackStatusIfNeeded(Playback::START, getLastReadPosition() / 1000);
    mStarted = true;

 (new AMessage(kWhatStart, this))->post();
}
