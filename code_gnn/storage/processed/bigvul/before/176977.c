void InputDispatcher::onANRLocked(
 nsecs_t currentTime, const sp<InputApplicationHandle>& applicationHandle,
 const sp<InputWindowHandle>& windowHandle,
 nsecs_t eventTime, nsecs_t waitStartTime, const char* reason) {
 float dispatchLatency = (currentTime - eventTime) * 0.000001f;
 float waitDuration = (currentTime - waitStartTime) * 0.000001f;
    ALOGI("Application is not responding: %s.  "
 "It has been %0.1fms since event, %0.1fms since wait started.  Reason: %s",
            getApplicationWindowLabelLocked(applicationHandle, windowHandle).string(),
            dispatchLatency, waitDuration, reason);

 time_t t = time(NULL);
 struct tm tm;
    localtime_r(&t, &tm);
 char timestr[64];
    strftime(timestr, sizeof(timestr), "%F %T", &tm);
    mLastANRState.clear();
    mLastANRState.append(INDENT "ANR:\n");
    mLastANRState.appendFormat(INDENT2 "Time: %s\n", timestr);
    mLastANRState.appendFormat(INDENT2 "Window: %s\n",
            getApplicationWindowLabelLocked(applicationHandle, windowHandle).string());
    mLastANRState.appendFormat(INDENT2 "DispatchLatency: %0.1fms\n", dispatchLatency);
    mLastANRState.appendFormat(INDENT2 "WaitDuration: %0.1fms\n", waitDuration);
    mLastANRState.appendFormat(INDENT2 "Reason: %s\n", reason);
    dumpDispatchStateLocked(mLastANRState);

 CommandEntry* commandEntry = postCommandLocked(
 & InputDispatcher::doNotifyANRLockedInterruptible);
    commandEntry->inputApplicationHandle = applicationHandle;
    commandEntry->inputWindowHandle = windowHandle;
    commandEntry->reason = reason;
}
