void NuPlayer::GenericSource::resume() {
    setDrmPlaybackStatusIfNeeded(Playback::START, getLastReadPosition() / 1000);
    mStarted = true;

 (new AMessage(kWhatResume, this))->post();
}
