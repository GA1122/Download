void MediaPlayerService::AudioOutput::setNextOutput(const sp<AudioOutput>& nextOutput) {
 Mutex::Autolock lock(mLock);
    mNextOutput = nextOutput;
}
