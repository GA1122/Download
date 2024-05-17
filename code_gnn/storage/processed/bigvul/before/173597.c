void NuPlayer::GenericSource::pause() {
    setDrmPlaybackStatusIfNeeded(Playback::PAUSE, 0);
    mStarted = false;
}
