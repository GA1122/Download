void StreamingProcessor::onFrameAvailable(const BufferItem&  ) {
    ATRACE_CALL();
 Mutex::Autolock l(mMutex);
 if (!mRecordingFrameAvailable) {
        mRecordingFrameAvailable = true;
        mRecordingFrameAvailableSignal.signal();
 }

}
