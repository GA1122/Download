void NuPlayer::GenericSource::restartPollBuffering() {
 if (mIsStreaming) {
        cancelPollBuffering();
        onPollBuffering();
 }
}
