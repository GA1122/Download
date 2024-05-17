void Camera3Device::RequestThread::setPaused(bool paused) {
 Mutex::Autolock l(mPauseLock);
    mDoPause = paused;
    mDoPauseSignal.signal();
}
