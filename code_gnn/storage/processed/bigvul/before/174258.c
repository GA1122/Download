void Camera3Device::RequestThread::requestExit() {
 Thread::requestExit();
    mDoPauseSignal.signal();
    mRequestSignal.signal();
}
