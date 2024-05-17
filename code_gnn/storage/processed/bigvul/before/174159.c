bool OMX::CallbackDispatcher::loop() {
 for (;;) {
        std::list<omx_message> messages;

 {
 Mutex::Autolock autoLock(mLock);
 while (!mDone && mQueue.empty()) {
                mQueueChanged.wait(mLock);
 }

 if (mDone) {
 break;
 }

            messages.swap(mQueue);
 }

        dispatch(messages);
 }

 return false;
}
