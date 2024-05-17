void NuPlayer::GenericSource::stop() {
    setDrmPlaybackStatusIfNeeded(Playback::STOP, 0);
    mStarted = false;
 if (mIsWidevine || mIsSecure) {
        sp<AMessage> msg = new AMessage(kWhatStopWidevine, this);
        sp<AMessage> response;
 (void) msg->postAndAwaitResponse(&response);
 }
}
