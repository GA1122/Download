bool SoundTriggerHwService::CallbackThread::threadLoop()
{
 while (!exitPending()) {
        sp<CallbackEvent> event;
        sp<SoundTriggerHwService> service;
 {
 Mutex::Autolock _l(mCallbackLock);
 while (mEventQueue.isEmpty() && !exitPending()) {
                ALOGV("CallbackThread::threadLoop() sleep");
                mCallbackCond.wait(mCallbackLock);
                ALOGV("CallbackThread::threadLoop() wake up");
 }
 if (exitPending()) {
 break;
 }
            event = mEventQueue[0];
            mEventQueue.removeAt(0);
            service = mService.promote();
 }
 if (service != 0) {
            service->onCallbackEvent(event);
 }
 }
 return false;
}
