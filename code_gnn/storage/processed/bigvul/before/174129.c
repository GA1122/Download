OMX::CallbackDispatcher::CallbackDispatcher(OMXNodeInstance *owner)
 : mOwner(owner),
      mDone(false) {
    mThread = new CallbackDispatcherThread(this);
    mThread->run("OMXCallbackDisp", ANDROID_PRIORITY_FOREGROUND);
}
