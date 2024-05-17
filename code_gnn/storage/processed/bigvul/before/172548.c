bool StreamingProcessor::threadLoop() {
 status_t res;

 {
 Mutex::Autolock l(mMutex);
 while (!mRecordingFrameAvailable) {
            res = mRecordingFrameAvailableSignal.waitRelative(
                mMutex, kWaitDuration);
 if (res == TIMED_OUT) return true;
 }
        mRecordingFrameAvailable = false;
 }

 do {
        res = processRecordingFrame();
 } while (res == OK);

 return true;
}
