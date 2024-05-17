void NuPlayer::GenericSource::stop() {
    setDrmPlaybackStatusIfNeeded(Playback::STOP, 0);
    mStarted = false;
 if (mIsWidevine) {
        sp<AMessage> msg = new AMessage(kWhatStopWidevine, id());
        sp<AMessage> response;
 (void) msg->postAndAwaitResponse(&response);
 }
}
