void InputDispatcher::setInputFilterEnabled(bool enabled) {
#if DEBUG_FOCUS
    ALOGD("setInputFilterEnabled: enabled=%d", enabled);
#endif

 {  
 AutoMutex _l(mLock);

 if (mInputFilterEnabled == enabled) {
 return;
 }

        mInputFilterEnabled = enabled;
        resetAndDropEverythingLocked("input filter is being enabled or disabled");
 }  

    mLooper->wake();
}
