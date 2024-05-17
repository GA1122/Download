void NuPlayer::GenericSource::cancelPollBuffering() {
    mBuffering = false;
 ++mPollBufferingGeneration;
    mPrevBufferPercentage = -1;
}
