bool CameraClient::lockIfMessageWanted(int32_t msgType) {
 int sleepCount = 0;
 while (mMsgEnabled & msgType) {
 if (mLock.tryLock() == NO_ERROR) {
 if (sleepCount > 0) {
                LOG1("lockIfMessageWanted(%d): waited for %d ms",
                    msgType, sleepCount * CHECK_MESSAGE_INTERVAL);
 }
 return true;
 }
 if (sleepCount++ == 0) {
            LOG1("lockIfMessageWanted(%d): enter sleep", msgType);
 }
        usleep(CHECK_MESSAGE_INTERVAL * 1000);
 }
    ALOGW("lockIfMessageWanted(%d): dropped unwanted message", msgType);
 return false;
}
