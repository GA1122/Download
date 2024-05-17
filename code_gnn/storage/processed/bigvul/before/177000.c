void InputDispatcher::setFocusedApplication(
 const sp<InputApplicationHandle>& inputApplicationHandle) {
#if DEBUG_FOCUS
    ALOGD("setFocusedApplication");
#endif
 {  
 AutoMutex _l(mLock);

 if (inputApplicationHandle != NULL && inputApplicationHandle->updateInfo()) {
 if (mFocusedApplicationHandle != inputApplicationHandle) {
 if (mFocusedApplicationHandle != NULL) {
                    resetANRTimeoutsLocked();
                    mFocusedApplicationHandle->releaseInfo();
 }
                mFocusedApplicationHandle = inputApplicationHandle;
 }
 } else if (mFocusedApplicationHandle != NULL) {
            resetANRTimeoutsLocked();
            mFocusedApplicationHandle->releaseInfo();
            mFocusedApplicationHandle.clear();
 }

#if DEBUG_FOCUS
#endif
 }  

    mLooper->wake();
}
